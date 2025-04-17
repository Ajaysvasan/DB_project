
# 🗄️ MiniDBMS - A Simple File-based C++ Database Management System

> A lightweight command-line Database Management System built entirely from scratch in C++ — with no external DB libraries or engines.

## 🚀 Features

- 📁 Create and manage multiple databases and tables
- 🧱 Insert, delete, update, and select data from tables
- 🛠️ File-based persistent storage (each table is a `.table` file)
- 🧠 Custom query parser and tokenizer
- ⚙️ Core C++ concepts like OOP, STL, File I/O, and modular code structure
- 🎯 Platform-independent logic with minimal dependencies

---

## 🧪 Supported Commands

### 📂 Database Commands
- `CREATE DATABASE <database_name>`
- `USE <database_name>`
- `CHANGE <database_name>`

### 📄 Table Commands
- `CREATE TABLE <table_name> (column_name TYPE, ...)`
- `INSERT INTO <table_name> VALUES (value1, value2, ...)`
- `DELETE FROM <table_name> WHERE <column> = <value>`
- `UPDATE <table_name> SET <column> = <value> WHERE <column> = <value>`
- `SELECT * FROM <table_name>`

---

## 🧱 Folder Structure

```
.
├── include/                 # Header files for DBMS and Parser
│   ├── dbms.h
│   └── Parser.h
├── src/
│   ├── dbms.cpp             # Command logic
│   └── Parser.cpp          # Tokenizer & query validation
├── main.cpp                 # CLI Entry point
├── databases/               # Generated databases and tables
└── README.md                # Project documentation
```

---

## ⚙️ How It Works

1. All commands are entered via command line (stdin).
2. The input is parsed using the custom tokenizer.
3. Based on command type, appropriate DBMS logic is executed.
4. Tables and databases are created/updated as files in the `./databases/` directory.

---

## 🖥️ How to Run

### 1️⃣ Clone the repo
```bash
git clone https://github.com/<your-username>/MiniDBMS.git
cd MiniDBMS
```

### 2️⃣ Compile
```bash
g++ main.cpp src/dbms.cpp src/Parser.cpp -o MiniDBMS

    OR use 
./start.bat

```


### 3️⃣ Run
```bash
./MiniDBMS
```

---

## ✍️ Example Usage

```
CREATE DATABASE College;
USE College;
CREATE TABLE Students (id INT, name TEXT, age INT);
INSERT INTO Students VALUES (1, "Ajay", 20);
SELECT * FROM Students;
```

---

## 📌 Goals of this Project

- Understand DBMS fundamentals
- Strengthen C++ design skills
- Build file-based persistent systems
- Implement real-world database features manually

---

## 🧠 Learnings

- Query tokenization and grammar handling
- Dynamic file-based table management
- Mapping C++ logic to real-world DBMS operations
- Debugging large modular systems

---

## 📬 Feedback & Contributions

Open to feedback, suggestions, and contributions! If you’d like to extend this with SQL grammar, indexing, or JOIN operations — feel free to fork and enhance.

---

## 🔗 Connect With Me

📧 [ajay192006@gmail.com](mailto:ajay192006@gmail.com)  
📎 [LinkedIn](https://www.linkedin.com/in/ajay-s-vasan-584111291/)  


---

## 🏁 Future Scope

- Add SELECT with conditions
- Add JOIN operations
- Add Indexing
- Build GUI on top (using Qt or web-based frontend)
