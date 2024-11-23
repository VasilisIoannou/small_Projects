package com.Vasilis.RealTimeChat.Chatroom;


import com.Vasilis.RealTimeChat.chat.ChatMessage;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/chatroom/")
@CrossOrigin
public class ChatRoomController {
    private final ChatroomRepository chatroomRepository;
    public ChatRoomController(ChatroomRepository chatroomRepository) {
        this.chatroomRepository = chatroomRepository;
    }

    @GetMapping("room/get/all")
    public List<ChatroomRecord> getAllChatrooms(){
        return chatroomRepository.getAllChat();
    }

    @GetMapping("messages/get/all")
    public List<ChatMessage> getAllMessages(){
        return chatroomRepository.getAllMessages();
    }

    @PutMapping("room/create")
    public void createChatroom(){
        chatroomRepository.createChatroom();
    }

    @PutMapping("messages/create/{sender_message_chatroom_senderId}")
    public void createMessage(@PathVariable String sender_message_chatroom_senderId){
        String sender = sender_message_chatroom_senderId.split("@")[0];
        String message = sender_message_chatroom_senderId.split("@")[1];
        Integer chatroomId = Integer.parseInt(sender_message_chatroom_senderId.split("@")[2]);
        Integer senderId = Integer.parseInt(sender_message_chatroom_senderId.split("@")[3]);
        chatroomRepository.createMessage(chatroomId,message, sender, senderId);
    }

}
