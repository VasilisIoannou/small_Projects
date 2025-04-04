package com.example.demo.Model;

import org.springframework.data.annotation.Id;
import java.util.List;

public record Content(
        @Id
        Integer id,
        String title,//Add NotBlank
        List<String> body
) {}
