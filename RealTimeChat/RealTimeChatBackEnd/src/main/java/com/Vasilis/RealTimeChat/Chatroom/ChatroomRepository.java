package com.Vasilis.RealTimeChat.Chatroom;

import com.Vasilis.RealTimeChat.accounts.AccountRepository;
import com.Vasilis.RealTimeChat.chat.ChatMessage;
import com.Vasilis.RealTimeChat.chat.MessageType;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

@Repository
public class ChatroomRepository {
    private final JdbcTemplate jdbcTemplate;

    public ChatroomRepository(JdbcTemplate jdbcTemplate, ObjectMapper objectMapper) {
        this.jdbcTemplate = jdbcTemplate;
    }

    private static ChatroomRecord chatroomMapRow(ResultSet rs, int rowNum) throws SQLException {
        return new ChatroomRecord(
                rs.getInt("id")
        );
    }

    private ChatMessage messagesMapRow(ResultSet rs, int rowNum) throws SQLException {
        return ChatMessage.builder()
                .id(rs.getInt("id"))
                .chatroomID(rs.getInt("chatroom_id"))
                .content(rs.getString("content"))
                .sender(rs.getString("sender"))
                .type(MessageType.valueOf(rs.getString("type")))
                .build();
    }

    public List<ChatroomRecord> getAllChat() {
        String sql = "SELECT * FROM chatrooms";
        List<ChatroomRecord> chatroomsList = jdbcTemplate.query(sql, ChatroomRepository::chatroomMapRow);
        return chatroomsList;
    }

    public List<ChatMessage> getAllMessages(){
        String sql = "SELECT * FROM messages";
        List<ChatMessage> messagesList = jdbcTemplate.query(sql,this::messagesMapRow);
        return messagesList;
    }

    public void createChatroom() {
        String sql = "INSERT INTO Chatrooms DEFAULT VALUES";
        jdbcTemplate.update(sql);
    }

    public void createMessage(Integer chatroomId,String content,String sender){
        String sql = "INSERT INTO messages (chatroom_id,content,sender) VALUES(?,?,?)";
        jdbcTemplate.update(sql,chatroomId,content,sender);
    }
}
