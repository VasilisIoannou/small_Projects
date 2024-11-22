-- Chatroom table
CREATE TABLE IF NOT EXISTS chatrooms (
  id INT AUTO_INCREMENT PRIMARY KEY
);

--Accounts table
CREATE TABLE IF NOT EXISTS accounts(
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255),
    password VARCHAR(255)
);

--ChatMessageTable
CREATE TABLE IF NOT EXISTS messages(
    id INT AUTO_INCREMENT PRIMARY KEY,
    chatroom_id INT NOT NULL,
    content TEXT NOT NULL,
    sender VARCHAR(255) NOT NULL,
    type VARCHAR(50),
    FOREIGN KEY (chatroom_id) REFERENCES chatrooms(id) ON DELETE CASCADE
)