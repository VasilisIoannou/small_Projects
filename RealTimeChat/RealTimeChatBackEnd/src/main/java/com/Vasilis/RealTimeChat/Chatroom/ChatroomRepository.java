package com.Vasilis.RealTimeChat.Chatroom;

import com.Vasilis.RealTimeChat.accounts.AccountRecord;
import com.Vasilis.RealTimeChat.chat.ChatMessage;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.support.GeneratedKeyHolder;
import org.springframework.jdbc.support.KeyHolder;
import org.springframework.stereotype.Repository;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

@Repository
public class ChatroomRepository {
    private final JdbcTemplate jdbcTemplate;

    public ChatroomRepository(JdbcTemplate jdbcTemplate, ObjectMapper objectMapper) {
        this.jdbcTemplate = jdbcTemplate;
    }

    private static ChatroomRecord chatroomMapRow(ResultSet rs, int rowNum) throws SQLException {
        return ChatroomRecord.builder()
                .id(rs.getInt("id"))
                .name(rs.getString("name"))
                .code(rs.getString("code"))
                .creatorID(rs.getInt("creatorId"))
                .build();
    }

    private ChatMessage messagesMapRow(ResultSet rs, int rowNum) throws SQLException {
        return ChatMessage.builder()
                .id(rs.getInt("id"))
                .chatroomID(rs.getInt("chatroom_id"))
                .userID(rs.getInt("sender_id"))
                .content(rs.getString("content"))
                .sender(rs.getString("sender"))
                .build();
    }

    private Boolean isCodeUnique(String code) {
        String sql = "SELECT COUNT(*) FROM chatrooms WHERE code = ?";
        Integer count = jdbcTemplate.queryForObject(sql,Integer.class,code);
        return count == 0;
    }

    private boolean isUserInChatroom(Integer chatroomID, Integer userID) {
        String sql = "SELECT COUNT(*) FROM chatrooms_accounts WHERE chatroomId = ? AND accountId = ?";
        Integer count = jdbcTemplate.queryForObject(sql, Integer.class, chatroomID, userID);
        return count != null && count > 0;
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

    public Integer createChatroom(String name,String code,Integer creatorId) {

        if(!isCodeUnique(code)){
            throw new IllegalArgumentException("Code must be unique");
        }

        String sql = "INSERT INTO Chatrooms (name, code, creatorId) VALUES (?, ?, ?)";

        KeyHolder keyHolder = new GeneratedKeyHolder();
        jdbcTemplate.update(connection ->   {
            PreparedStatement ps = connection.prepareStatement(sql,new String[]{"id"});
            ps.setString(1,name);
            ps.setString(2,code);
            ps.setInt(3,creatorId);
            return ps;
        }, keyHolder);
        Number generatedKey =  keyHolder.getKey();
        if(generatedKey == null) {
            throw new IllegalStateException("Failed to retrieve generated key");
        }
        return generatedKey.intValue();
    }

    public void addAccountToChatroom(Integer chatroomID, Integer userID) {
        if (chatroomID == null || userID == null) {
            throw new IllegalArgumentException("chatroomID and userID cannot be null");
        }
        if (isUserInChatroom(chatroomID, userID)) {
            System. out.println("User is already part of the chatroom.");
            return;
        }
        //Check if account or chatroom does not exists

        String sql = "INSERT INTO chatrooms_accounts (chatroomID, accountID) VALUES (?, ?)";
        jdbcTemplate.update(sql,chatroomID,userID);
    }

    public void addAcountToChatroomByCode(String code, Integer userID) {
        if (code == null || userID == null) {
            throw new IllegalArgumentException("code and userID cannot be null");
        }

        //get chatroomID
        List<ChatroomRecord> chatroomIDList = getChatroomByCode(code);

        if(chatroomIDList == null || chatroomIDList.isEmpty()){
            throw new IllegalArgumentException("That chatroom doesnt't exists");
        }

        Integer chatroomId = chatroomIDList.get(0).getId();

        if (isUserInChatroom(chatroomId, userID)) {
            System. out.println("User is already part of the chatroom.");
            return;
        }

        addAccountToChatroom(chatroomId, userID);



    }

    public List<ChatroomRecord> getChatroomsByAccount(Integer accountId) {//Fix This
        String sql = "SELECT chatroomId FROM chatrooms_accounts WHERE accountId = ?";
        List<Integer> chatroomIds  = jdbcTemplate.queryForList(sql,Integer.class,accountId);
        if (chatroomIds.isEmpty()) {
            return new ArrayList<>();
        }

        String chatroomSql = "SELECT * FROM chatrooms WHERE id IN (" + String.join(",", Collections.nCopies(chatroomIds.size(), "?")) + ")";
        Object[] params = chatroomIds.toArray();
        List<ChatroomRecord> chatrooms = jdbcTemplate.query(chatroomSql, new BeanPropertyRowMapper<>(ChatroomRecord.class), params);

        return chatrooms;
    }


    public List<ChatroomRecord> getChatroomByCode(String code) {
        String sql = "SELECT * FROM chatrooms WHERE code = ?";
        List<ChatroomRecord> Chatroom = jdbcTemplate.query(sql,ChatroomRepository::chatroomMapRow,code);
        return Chatroom;
    }

    public void createMessage(Integer chatroomId,String content,String sender,Integer sender_id){
        String sql = "INSERT INTO messages (chatroom_id,content,sender_id,sender) VALUES(?,?,?,?)";
        jdbcTemplate.update(sql,chatroomId,content,sender_id,sender);
    }

    public List<ChatMessage> getMessagesByChatroomId(Integer chatroomId){
        String sql = "SELECT * FROM messages WHERE chatroom_id = ?";
        List<ChatMessage> messages = jdbcTemplate.query(sql,this::messagesMapRow,chatroomId);
        return messages;
    }

    public void deleteChatroom(Integer chatroomId) {
        //Delete from chatrooms-accounts
        String sqlAcounts_chatrooms = "DELETE FROM chatrooms_accounts WHERE chatroomId = ?";
        jdbcTemplate.update(sqlAcounts_chatrooms,chatroomId);
        //Delete from chatrooms
        String sqlChatrooms = "DELETE FROM chatrooms WHERE id = ?";
        int rowsAffected = jdbcTemplate.update(sqlChatrooms,chatroomId);

        if (rowsAffected == 0) {
            throw new IllegalArgumentException("Chatroom with ID " + chatroomId + " not found.");
        }

    }
}
