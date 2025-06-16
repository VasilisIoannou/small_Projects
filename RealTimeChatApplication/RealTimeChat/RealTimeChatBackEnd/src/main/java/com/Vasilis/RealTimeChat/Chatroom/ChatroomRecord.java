package com.Vasilis.RealTimeChat.Chatroom;

import com.Vasilis.RealTimeChat.accounts.AccountRecord;
import lombok.*;
import org.springframework.data.relational.core.sql.In;

import java.util.ArrayList;
import java.util.List;

@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
@Builder
public class ChatroomRecord {
    Integer id;
    String name;
    String code;//Make Not Null
    Integer creatorID;
}
