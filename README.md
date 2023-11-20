# tgbot
todo-list telegram bot

### database structure:
```SQL
CREATE TABLE "Users" (
	"UserID"	INTEGER NOT NULL UNIQUE,
	"IsAdmin"	BOOLEAN NOT NULL DEFAULT 0,
  "Language" TEXT NOT NULL DEFAULT 'en',
	PRIMARY KEY("UserID" AUTOINCREMENT)
);

CREATE TABLE "Water" (
	"UserID"	INTEGER NOT NULL,
	"Date"	DATETIME NOT NULL,
	"WaterDrank"	INTEGER NOT NULL,
	FOREIGN KEY("UserID") REFERENCES "Users" ("UserID")
);

CREATE TABLE "ToDoList" (
	"UserID"	INTEGER NOT NULL UNIQUE,
	"Notes"	TEXT,
	FOREIGN KEY("UserID") REFERENCES "Users" ("UserID")
);
```