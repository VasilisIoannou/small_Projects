package com.Vasilis.RealTimeChat.chat;

import lombok.*;

import java.awt.*;
@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
@Builder
public class ChatMessage {
    private Integer id;
    private Integer chatroomID;
    private Integer userID;
    private String content;
    private String sender;
}
