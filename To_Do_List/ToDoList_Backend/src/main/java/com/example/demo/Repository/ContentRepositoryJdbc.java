package com.example.demo.Repository;

import com.example.demo.Model.Content;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.support.GeneratedKeyHolder;
import org.springframework.jdbc.support.KeyHolder;
import org.springframework.stereotype.Repository;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Arrays;
import java.util.List;


@Repository
public class ContentRepositoryJdbc {

    public final JdbcTemplate jdbcTemplate;
    private final ObjectMapper objectMapper;

    public ContentRepositoryJdbc(JdbcTemplate jdbcTemplate, ObjectMapper objectMapper) {
        this.jdbcTemplate = jdbcTemplate;
        this.objectMapper = objectMapper;
    }

    private static Content mapRow(ResultSet rs, int rowNum) throws SQLException {

        ObjectMapper objectMapper = new ObjectMapper();
       String bodyJson = rs.getString("body");

          List<String> body = null;


        try {
            // Deserialize JSON to List<String>
            bodyJson = objectMapper.readValue(bodyJson, String.class);

            body = objectMapper.readValue(bodyJson, new TypeReference<List<String>>() {});
        } catch (JsonProcessingException e) {
            System.out.println("Json processing error");
            body = List.of(); // Default to an empty list if parsing fails
        }

        return new Content(
            rs.getInt("id"),
            rs.getString( "title"),
            body);
    }

    public List<Content> getAllContent(){
        String sql = "SELECT * FROM content";
        List<Content> contents =  jdbcTemplate . query(sql, ContentRepositoryJdbc::mapRow);
        return contents;
    }

    public Integer createContent(String title,List<String> body){
        String sql = "INSERT INTO content (title,body) VALUES (?,?)";
        String bodyJson;

        try {
            bodyJson = objectMapper.writeValueAsString(body); // Serialize List<String> to JSON
        } catch (JsonProcessingException e) {
            throw new RuntimeException("Failed to serialize body to JSON", e); // Handle serialization exception
        }

        KeyHolder keyHolder = new GeneratedKeyHolder();
        jdbcTemplate.update(connection -> {
            PreparedStatement ps = connection.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);
            ps.setString(1, title);
            ps.setString(2, bodyJson);
            return ps;
        }, keyHolder);

        return keyHolder.getKey().intValue(); // Return the generated ID
    }


    public void updateContent(int id,String title,List<String> body){
        String sql = "UPDATE content SET title=?,body=? WHERE id=?";
        try {
            String bodyJson = objectMapper.writeValueAsString(body); // Serialize List<String> to JSON
            jdbcTemplate.update(sql, title, bodyJson, id);

        } catch (JsonProcessingException e) {
            throw new RuntimeException("Failed to serialize body to JSON", e); // Handle serialization exception
        }
    }

    public void deleteContent(int id){
        String sql = "DELETE FROM content WHERE id=?";
        jdbcTemplate.update(sql,id);
    }

    public Content getContentById(int id){
        String sql = "SELECT * FROM content WHERE id=?";
        Content content = jdbcTemplate.queryForObject(sql, ContentRepositoryJdbc::mapRow,id);
        return content;
    }

    public List<Content>  getContentByTitle(String title){
        String sql = "SELECT * FROM content WHERE title=?";
        List<Content> ContentList = jdbcTemplate.query(sql, ContentRepositoryJdbc::mapRow,title);
        return ContentList;
    }

//    public List<Content>  getContentByCharInTitle(Character c){
//        String sql = "SELECT * FROM content WHERE title LIKE ?";
//        String param = "%" + c + "%"; // Create the parameter string with wildcards
//        List<Content> ContentList = jdbcTemplate.query(sql, ContentRepositoryJdbc::mapRow,param);
//        return ContentList;
//    }

    public List<Content>  getContentByBody(String s){
        String sql = "SELECT * FROM content WHERE body LIKE ?";
        String param = "%" + s + "%";
        List<Content> ContentList = jdbcTemplate.query(sql, ContentRepositoryJdbc::mapRow,param);
        return ContentList;
    }


    public boolean existsById(int id){
        String sql = "SELECT COUNT(*) FROM content WHERE id=?";
        Integer count= jdbcTemplate.queryForObject(sql, Integer.class,id);
        return  count !=null && count > 0;
    }


}