import asyncio
import socket
import time
import random

SERVER_IP = "127.0.0.1"
SERVER_PORT = 9122

def send_message_sync(message: str) -> str:
    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.settimeout(2.0)
        client_socket.connect((SERVER_IP, SERVER_PORT))
        client_socket.sendall(message.encode('utf-8'))
        response = client_socket.recv(4096).decode('utf-8').strip()
        print(f"[+] Отправлено: '{message.strip()}' | Ответ сервера: '{response}'")
        return response
    except Exception as e:
        print(f"[-] Ошибка: {e}")
    finally:
        try: client_socket.close()
        except: pass
    return ""

async def async_worker(connection_id: int, requests_per_conn: int, base_value: str, tracking_dict: dict):
    """
    Имитирует поведение реального клиента:
    Отправляет команду -> Ждет ответ от БД -> Делает микро-паузу
    """
    try:
        reader, writer = await asyncio.open_connection(SERVER_IP, SERVER_PORT)
        
        for req_id in range(requests_per_conn):
            # Генерируем уникальный ключ
            unique_key = f"key_{connection_id}_{req_id}_{random.randint(100, 999)}"
            payload = f"set {unique_key} {base_value}\n"
            
            # 1. Отправляем запрос
            writer.write(payload.encode('utf-8'))
            await writer.drain()
            
            # 2. Ждем ответ от БД (как это делает реальный драйвер)
            response_data = await reader.read(1024)
            response = response_data.decode('utf-8').strip()
            
            # 3. Если база подтвердила запись, заносим в локальный трекер
            if response in ["Created", "Updated"]:
                tracking_dict[unique_key] = True
            
            # 4. Имитируем реальную задержку работы приложения (от 1 до 5 миллисекунд)
            await asyncio.sleep(random.uniform(0.001, 0.005))
            
        writer.close()
        await writer.wait_closed()
        return True
    except Exception:
        return False

async def verify_keys_async(keys_to_check: list, expected_value: str):
    """Безопасная асинхронная верификация записанных данных"""
    print(f"\n[•] Запуск верификации данных...")
    print(f"[•] Проверяем сохранность {len(keys_to_check)} ключей...")
    
    lost_keys = 0
    start_time = time.time()
    
    # Пул одновременных соединений для проверки, чтобы не вешать сеть
    semaphore = asyncio.Semaphore(100)
    
    async def check_key(key):
        nonlocal lost_keys
        async with semaphore:
            try:
                reader, writer = await asyncio.open_connection(SERVER_IP, SERVER_PORT)
                writer.write(f"get {key}\n".encode('utf-8'))
                await writer.drain()
                data = await reader.read(1024)
                response = data.decode('utf-8').strip()
                writer.close()
                await writer.wait_closed()
                
                if response != expected_value:
                    lost_keys += 1
            except:
                lost_keys += 1

    tasks = [check_key(key) for key in keys_to_check]
    await asyncio.gather(*tasks)
        
    end_time = time.time()
    print("=" * 40)
    print(f"[✓] Проверка завершена за {end_time - start_time:.2f} сек.")
    if lost_keys == 0:
        print(f"[🏆] ИДЕАЛЬНО! Потерь нет. Все данные (100%) на месте.")
    else:
        print(f"[❌] ОБНАРУЖЕНЫ ПОТЕРИ: {lost_keys} из {len(keys_to_check)} ключей пропали!")
    print("=" * 40)

async def run_stress_test(connections: int, requests_per_conn: int, value: str):
    print(f"\n[!] Запуск реалистичного теста нагрузки...")
    print(f"[•] Активных клиентов: {connections}")
    print(f"[•] Операций на клиента: {requests_per_conn}")
    print(f"[•] Всего ключей летит в ДБ: {connections * requests_per_conn}")
    
    start_time = time.time()
    tracking_dict = {} # Сюда пишем только то, что сервер РЕАЛЬНО подтвердил
    
    tasks = [async_worker(i, requests_per_conn, value, tracking_dict) for i in range(connections)]
    results = await asyncio.gather(*tasks)
    end_time = time.time()
    
    success = sum(1 for r in results if r)
    
    print("\n" + "=" * 40)
    print(f"[✓] Сессия успешно завершена за {end_time - start_time:.2f} сек.")
    print(f"[✓] Успешных сессий клиентов: {success}/{connections}")
    print(f"[✓] Сервером успешно подтверждено: {len(tracking_dict)} записей SET")
    print("=" * 40)
    
    if tracking_dict:
        await verify_keys_async(list(tracking_dict.keys()), value)
    else:
        print("[-] Нет данных для верификации.")

def main():
    while True:
        print("\n" + "=" * 40)
        print(f" РЕАЛИСТИЧНЫЙ ТЕСТЕР ДБ ({SERVER_IP}:{SERVER_PORT})")
        print("=" * 40)
        print("1. Отправить один запрос / Проверить статус")
        print("2. Запустить реалистичный тест + верификация")
        print("0. Exit")
        print("=" * 40)
        choice = input("Выберите действие: ").strip()
        if choice == "1":
            msg = input("Введите команду: ")
            send_message_sync(msg if msg.endswith('\n') else msg + '\n')
        elif choice == "2":
            try:
                conns = int(input("Количество одновременных клиентов (например, 500): "))
                reqs = int(input("Количество операций на одного клиента (например, 20): "))
                val = input("Какое значение записывать (value): ").strip()
                asyncio.run(run_stress_test(conns, reqs, val))
            except ValueError:
                print("[-] Ошибка: Вводите только целые числа!")
        elif choice == "0":
            break

if __name__ == "__main__":
    main()
