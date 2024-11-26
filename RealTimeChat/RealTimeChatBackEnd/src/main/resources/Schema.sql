--Accounts table
CREATE TABLE IF NOT EXISTS accounts(
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255) UNIQUE ,
    password VARCHAR(255)
);

-- Chatroom table
CREATE TABLE IF NOT EXISTS chatrooms (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name TEXT NOT NULL,
    code TEXT NOT NULL UNIQUE ,
    creatorId INT,
    FOREIGN KEY (creatorId) REFERENCES accounts(id) ON DELETE CASCADE
);

--Joint Table for chatrooms and accounts
CREATE TABLE IF NOT EXISTS chatrooms_accounts (
    chatroomId INT,
    accountId INT,
    PRIMARY KEY (chatroomId, accountId),
    FOREIGN KEY (chatroomId) REFERENCES chatrooms(id) ON DELETE CASCADE,
    FOREIGN KEY (accountId) REFERENCES accounts(id) ON DELETE CASCADE
);

--ChatMessageTable
CREATE TABLE IF NOT EXISTS messages(
    id INT AUTO_INCREMENT PRIMARY KEY,
    chatroom_id INT NOT NULL,
    sender_id INT NOT NULL,
    content TEXT NOT NULL,
    sender VARCHAR(255) NOT NULL,
    FOREIGN KEY (chatroom_id) REFERENCES chatrooms(id) ON DELETE CASCADE,
    FOREIGN KEY (sender_id) REFERENCES accounts(id) ON DELETE CASCADE
)