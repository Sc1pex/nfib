package db

import (
	"database/sql"
	"fmt"
	"log"

	_ "github.com/tursodatabase/go-libsql"
)

type Db struct {
	db *sql.DB
}

func Open() *Db {
	db, err := sql.Open("libsql", "file:sqlite/nfib.db")
	if err != nil {
		log.Fatal("Failed to open database: ", err)
	}

	return &Db{db}
}

func (db *Db) Setup(current_impls []string) {
	for _, i := range current_impls {
		create := fmt.Sprintf(`CREATE TABLE IF NOT EXISTS %s (
            idx INTEGER PRIMARY KEY,
            avg_time REAL NOT NULL,
            max_time REAL NOT NULL,
            min_time REAL NOT NULL,
			result STRING NOT NULL
        )`, i)

		_, err := db.db.Exec(create, i)
		if err != nil {
			log.Fatal("Failed to create table for ", i, ": ", err)
		}
	}
}

func (db *Db) Close() error {
	return db.db.Close()
}

type Entry struct {
	Idx      uint64
	Avg_time float64
	Min_time float64
	Max_time float64
	Result   string
}

func (db *Db) GetIdx(idx uint64, impl string) *Entry {
	query := fmt.Sprintf("SELECT * from %s WHERE idx = ?", impl)

	var entry Entry
	if err := db.db.QueryRow(query, idx).Scan(&entry.Idx, &entry.Avg_time, &entry.Max_time, &entry.Min_time, &entry.Result); err != nil {
		if err == sql.ErrNoRows {
			return nil
		}
		log.Fatal("Failed to get ", idx, "from ", impl, ": ", err)
	}

	return &entry
}

func (db *Db) GetAll(impl string) []Entry {
	query := fmt.Sprintf("SELECT * from %s", impl)

	rows, err := db.db.Query(query)
	if err != nil {
		return nil
	}
	defer rows.Close()

	var result []Entry
	for rows.Next() {
		var entry Entry
		if err := rows.Scan(&entry.Idx, &entry.Avg_time, &entry.Max_time, &entry.Min_time, &entry.Result); err != nil {
			log.Fatal("Failed to get entries in ", impl, ": ", err)
		}
		result = append(result, entry)
	}

	return result

}

func (db *Db) Add(entry Entry, impl string) {
	query := fmt.Sprintf("INSERT INTO %s (idx, avg_time, min_time, max_time, result) VALUES (?, ?, ?, ?, ?)", impl)

	if _, err := db.db.Exec(query, entry.Idx, entry.Avg_time, entry.Min_time, entry.Max_time, entry.Result); err != nil {
		log.Fatal("Failed to add ", entry, "to table ", impl, ": ", err)
	}
}
