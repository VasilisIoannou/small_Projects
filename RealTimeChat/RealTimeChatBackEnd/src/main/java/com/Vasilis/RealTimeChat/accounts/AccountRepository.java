package com.Vasilis.RealTimeChat.accounts;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

@Repository
public class AccountRepository {
    public final JdbcTemplate jdbcTemplate;
    //private final ObjectMapper objectMapper;

    public AccountRepository(JdbcTemplate jdbcTemplate, ObjectMapper objectMapper) {
        this.jdbcTemplate = jdbcTemplate;
        //this.objectMapper = objectMapper;
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

    public List<AccountRecord> findByUsernameAndPassword(String username, String password) {
        String sql = "SELECT * FROM accounts WHERE username = ? AND password = ?";
        List<AccountRecord> accounts = jdbcTemplate.query(sql, AccountRepository::mapRow, username, password);
        return accounts;
    }

    public void createAccount(String username, String password) {
        String sql = "INSERT INTO accounts (username, password) VALUES (?, ?)";
        jdbcTemplate.update(sql, username, password);
    }
}
