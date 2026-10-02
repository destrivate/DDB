# DDB — Fast In-Memory Key-Value DB

Fast database in C++ for Linux.

## 📊 Benchmark
* **Load:** 1000 connects / 200 000 commands `set`
* **Speed:** ~260 000 RPS (0.77 s)

## 🛠️ Build and Start


### Assembly:
```bash
./build.sh
```

*Put file `default.txt` (format `key|value`) to the root of the binary, if you want to load default data. If they are not needed — it is not necessary to create a file.*

## 🔌 Commands (TCP, Port 9122)

* `set <key> <value>` — write/update
  * Returns: `Created` (when created) or `Updated` (when updated)
* `get <ключ>` — read
  * Returns: key value or `NoneValue` (if no key)
* `del <ключ>` — delete
  * Returns: `Success`

For syntax errors, it returns `SyntaxError`.
