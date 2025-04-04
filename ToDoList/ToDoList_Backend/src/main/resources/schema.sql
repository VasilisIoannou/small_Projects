CREATE TABLE IF NOT EXISTS Content (
    id INTEGER AUTO_INCREMENT PRIMARY KEY,
    title TEXT,
    body JSON
);

INSERT INTO Content(title,body)
VALUES ('Clean your room','["Sira1", "Sira2"]');
INSERT INTO Content(title,body)
VALUES ('Hello','["Sira 1", "Sira 2"]');

