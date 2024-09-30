package db

import (
	"database/sql"
	"fmt"
	"log"
	"nfib/data"

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
            input INTEGER PRIMARY KEY,
			output STRING NOT NULL,
			
            avg_time REAL NOT NULL,
            min_time REAL NOT NULL,
            max_time REAL NOT NULL,

			num_runs INTEGER NOT NULL
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

func (db *Db) GetIdx(idx uint64, impl string) *data.RunResult {
	query := fmt.Sprintf("SELECT * from %s WHERE input = ?", impl)

	var entry data.RunResult
	err := db.db.QueryRow(query, idx).Scan(
		&entry.Input,
		&entry.Output,
		&entry.AvgTime,
		&entry.MinTime,
		&entry.MaxTime,
		&entry.NumRuns,
	)
	if err != nil {
		if err == sql.ErrNoRows {
			return nil
		}
		log.Fatal("Failed to get ", idx, "from ", impl, ": ", err)
	}

	return &entry
}

func (db *Db) GetAll(impl string) []data.RunResult {
	query := fmt.Sprintf("SELECT * from %s", impl)

	rows, err := db.db.Query(query)
	if err != nil {
		return nil
	}
	defer rows.Close()

	var result []data.RunResult
	for rows.Next() {
		var entry data.RunResult
		err := rows.Scan(
			&entry.Input,
			&entry.Output,
			&entry.AvgTime,
			&entry.MinTime,
			&entry.MaxTime,
			&entry.NumRuns,
		)
		if err != nil {
			log.Fatal("Failed to get entries in ", impl, ": ", err)
		}
		result = append(result, entry)
	}

	return result
}

func (db *Db) DeleteAll(impl string) {
	query := fmt.Sprintf("DELETE FROM %s", impl)

	_, err := db.db.Exec(query)
	if err != nil {
		log.Fatal("Failed to delete from db ", impl, ": ", err)
	}
}

func (db *Db) Add(entry data.RunResult, impl string) {
	query := fmt.Sprintf(`INSERT INTO %s 
		(input, output, avg_time, min_time, max_time, num_runs) 
		VALUES (?, ?, ?, ?, ?, ?)
	`, impl)

	_, err := db.db.Exec(query,
		entry.Input,
		entry.Output,
		entry.AvgTime,
		entry.MinTime,
		entry.MaxTime,
		entry.NumRuns,
	)
	if err != nil {
		log.Fatal("Failed to add ", entry, "to table ", impl, ": ", err)
	}
}
