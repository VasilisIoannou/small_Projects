package com.Vasilis.RealTimeChat.accounts;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.support.GeneratedKeyHolder;
import org.springframework.jdbc.support.KeyHolder;
import org.springframework.stereotype.Repository;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

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
        List<AccountRecord> accounts = jdbcTemplate.query(sql, AccountRepository::mapRow);
        return accounts;
    }

    public List<AccountRecord> getAccountById(Integer id) {
        String sql = "SELECT * FROM accounts WHERE id = ?";
        List<AccountRecord> accounts = jdbcTemplate.query(sql, AccountRepository::mapRow, id);
        return accounts;
    }

    public List<AccountRecord> findByUsernameAndPassword(String username, String password) {
        String sql = "SELECT * FROM accounts WHERE username = ? AND password = ?";
        List<AccountRecord> accounts = jdbcTemplate.query(sql, AccountRepository::mapRow, username, password);
        return accounts;
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
        jdbcTemplate.update(sql, newPassword, id);
    }

}
