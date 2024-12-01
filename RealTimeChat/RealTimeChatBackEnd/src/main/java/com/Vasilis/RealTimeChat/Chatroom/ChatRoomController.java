package com.Vasilis.RealTimeChat.Chatroom;


import com.Vasilis.RealTimeChat.chat.ChatMessage;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/chatroom/")
@CrossOrigin(origins = "http://localhost:5173") // Allow requests from the frontend
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

    @GetMapping("room/get/code/{code}")
    public List<ChatroomRecord> getChatroomByCode(@PathVariable String code){
        return chatroomRepository.getChatroomByCode(code);
    }

    @PutMapping("room/create/{name_code_creatorId}")
    public Integer createChatroom(@PathVariable String name_code_creatorId){
        String name = name_code_creatorId.split("@")[0];
        String code = name_code_creatorId.split("@")[1];
        Integer creatorId = Integer.parseInt(name_code_creatorId.split("@")[2]);
        return  chatroomRepository.createChatroom(name,code,creatorId);
    }

    @PutMapping("messages/create/{sender_message_chatroom_senderId}")
    public void createMessage(@PathVariable String sender_message_chatroom_senderId){
        String sender = sender_message_chatroom_senderId.split("@")[0];
        String message = sender_message_chatroom_senderId.split("@")[1];
        Integer chatroomId = Integer.parseInt(sender_message_chatroom_senderId.split("@")[2]);
        Integer senderId = Integer.parseInt(sender_message_chatroom_senderId.split("@")[3]);
        chatroomRepository.createMessage(chatroomId,message, sender, senderId);
    }

    @PutMapping("/account_chatroom/createConnection/{accountId_chatroomId}")
    public void addAccountToChatroom(@PathVariable String accountId_chatroomId){
        Integer senderId = Integer.parseInt(accountId_chatroomId.split("@")[0]);
        Integer chatroomId = Integer.parseInt(accountId_chatroomId.split("@")[1]);
        System.out.println(chatroomId+" "+senderId);

        chatroomRepository.addAccountToChatroom(chatroomId,senderId);
    }

    @GetMapping("/account_chatroom/getChatroomsByAccount/{accountIdStr}")
    public List<ChatroomRecord> getChatroomsByAccount(@PathVariable String accountIdStr){
        Integer accountId = Integer.parseInt(accountIdStr);
        return chatroomRepository.getChatroomsByAccount(accountId);
    }

    @PutMapping("/account_chatroom/addAcountToChatroomByCode/{code_userId}")
    public void addAcountToChatroomByCode(@PathVariable String code_userId){
        String code = code_userId.split("@")[0];
        Integer userId = Integer.parseInt(code_userId.split("@")[1]);
        chatroomRepository.addAcountToChatroomByCode(code,userId);
    }  
}
