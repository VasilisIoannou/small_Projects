package com.Vasilis.RealTimeChat.accounts;

import com.Vasilis.RealTimeChat.Chatroom.ChatroomRecord;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.data.relational.core.sql.In;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.namedparam.NamedParameterJdbcTemplate;
import org.springframework.jdbc.support.GeneratedKeyHolder;
import org.springframework.jdbc.support.KeyHolder;
import org.springframework.stereotype.Repository;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Collections;
import java.util.List;
import java.util.Map;

@Repository
public class AccountRepository {
    public final JdbcTemplate jdbcTemplate;

    public AccountRepository(JdbcTemplate jdbcTemplate, ObjectMapper objectMapper) {
        this.jdbcTemplate = jdbcTemplate;
    }

    public static AccountRecord mapRow(ResultSet rs,Integer rowNumber) throws SQLException {
        return new AccountRecord(
            rs.getInt("id"),
            rs.getString("username"),
            rs.getString("password")
        );
    }

    public List<AccountRecord> getAll() {
        String sql = "SELECT * FROM accounts";
        return jdbcTemplate.query(sql, AccountRepository::mapRow);
    }

    public List<AccountRecord> getAccountById(Integer id) {
        String sql = "SELECT * FROM accounts WHERE id = ?";
        return jdbcTemplate.query(sql, AccountRepository::mapRow, id);
    }

    public List<AccountRecord> findByUsernameAndPassword(String username, String password) {
        String sql = "SELECT * FROM accounts WHERE username = ? AND password = ?";
        return jdbcTemplate.query(sql, AccountRepository::mapRow, username, password);
    }

    public Integer createAccount(String username, String password) {
        String sql = "INSERT INTO accounts (username, password) VALUES (?, ?)";

        KeyHolder keyHolder = new GeneratedKeyHolder();
        jdbcTemplate.update(connection ->   {
            PreparedStatement ps = connection.prepareStatement(sql,new String[]{"id"});
            ps.setString(1,username);
            ps.setString(2,password);
            return ps;
        }, keyHolder);
        Number generatedKey =  keyHolder.getKey();
        if(generatedKey == null) {
            throw new IllegalStateException("Failed to retrieve generated key");
        }
        return generatedKey.intValue();
    }

    public void deleteAccount(Integer id) {
        String sql = "DELETE FROM accounts WHERE id = ?";
        jdbcTemplate.update(sql, id);
    }

    public void changeAccountName(Integer id, String newUsername) {
        String sql = "UPDATE accounts SET username = ? WHERE id = ?";
        jdbcTemplate.update(sql, newUsername, id);
    }

    public void changeAccountPassword(Integer id, String newPassword) {
        String sql = "UPDATE accounts SET password = ? WHERE id = ?";
        jdbcTemplate.update(sql,newPassword,id);
    }

    private boolean isUserAlreadyFriend(Integer userId, Integer friendId) {
        String sql = "SELECT COUNT(*) FROM friends WHERE userId = ? AND friendId = ?";
        Integer count = jdbcTemplate.queryForObject(sql, Integer.class, userId, friendId);
        return count != null && count > 0;
    }

    public void addAccountToFriends(Integer userId, Integer friendId) {
        if (friendId == null || userId == null) {
            throw new IllegalArgumentException("userId and friendId cannot be null");
        }
        if (friendId.equals(userId)) {
            throw new IllegalArgumentException("userId and friendId cannot be the same");
        }
        if (isUserAlreadyFriend(userId,friendId)){
            System. out.println("User is already a friend.");
            return;
        }

        String sql = "INSERT INTO friends (userId, friendId) VALUES (?, ?)";
        jdbcTemplate.update(sql,userId,friendId);
    }

    public List<AccountRecord> getFriends(Integer userId){//Check if this works

        String sqlGetFriendsCombined = """
                                        SELECT friendId AS friendId FROM friends WHERE userId = ?
                                        UNION
                                        SELECT userId AS friendId FROM friends  WHERE friendId = ?
                                        """;

        List<Integer> friendsIDs = jdbcTemplate.queryForList(sqlGetFriendsCombined, Integer.class, userId,userId);

        if(friendsIDs.isEmpty()) {
            return Collections.emptyList();
        }

        NamedParameterJdbcTemplate namedParameterJdbcTemplate = new NamedParameterJdbcTemplate(jdbcTemplate.getDataSource());

        String sqlGetFriendsData = "SELECT * FROM accounts WHERE id IN (:friendIds)";
        Map<String, List<Integer>> paramMap = Collections.singletonMap("friendIds", friendsIDs);

        return namedParameterJdbcTemplate.query(sqlGetFriendsData, paramMap, AccountRepository::mapRow);
    }

    public List<AccountRecord> getMutualFriends(Integer user1Id, Integer user2Id){//FIX
        String sqlUSER = """ 
                       SELECT friendId AS friendId FROM friends WHERE userId = ?
                       UNION
                       SELECT userId AS friendId FROM friends WHERE friendId = ?
                      """;
        List<Integer> user1_friendsIDs = jdbcTemplate.queryForList(sqlUSER, Integer.class, user1Id,user1Id);
        List<Integer> user2_friendsIDs = jdbcTemplate.queryForList(sqlUSER, Integer.class, user2Id,user2Id);

        user1_friendsIDs.retainAll(user2_friendsIDs);
        List<Integer> toRemove = List.of(user1Id, user2Id);
        user1_friendsIDs.removeAll(toRemove);

        if(user1_friendsIDs.isEmpty()) {
            return Collections.emptyList();
        }

        NamedParameterJdbcTemplate namedParameterJdbcTemplate = new NamedParameterJdbcTemplate(jdbcTemplate.getDataSource());

        String sqlGetMutualFriendsData = "SELECT * FROM accounts WHERE id IN (:user1_friendsIDs)";
        Map<String, List<Integer>> paramMap = Collections.singletonMap("friendIds", user1_friendsIDs);

        return namedParameterJdbcTemplate.query(sqlGetMutualFriendsData, paramMap, AccountRepository::mapRow);
    }

}
