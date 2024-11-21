import React, {useState } from 'react'
import { useStompClient, useSubscription } from "react-stomp-hooks";

import style from '../Styles/ChatRoomComponent.module.css'


const ChatRoomComponent = ({usernameLogIn}) => {

  const [messages, setMessages] = useState([]);
  const [newMessage,setNewMessage] = useState("");

  const stompClient = useStompClient(); 

  useSubscription("/topic/public",(message) => {
    const parseMessage = JSON.parse(message.body);
    setMessages((prev) => [...prev,parseMessage]);
  })

  function sendMessage(){
    console.log(newMessage);
    if(stompClient && newMessage.trim() !==''){
        stompClient.publish({
            destination: '/app/chat.sendMessage',
            
            body: JSON.stringify({
                content: newMessage || "",
                sender: usernameLogIn || "Anonymous",
             })
        });

        setNewMessage("");
    }
  }

  return (
    <>
    <div id="MessageBox">
        {usernameLogIn !== '' && usernameLogIn  ? usernameLogIn : "Please Log In"}
        <div className={style.messageBox}>
            {messages.map((msg,index)=>{
                if(usernameLogIn === msg.sender){ //Your own messages
                  return(
                    <div key={index} className={style.message}>
                      <div>Your message</div>
                      <div>{msg.content}</div>
                      <div className={style.user}>{msg.sender}</div>
                    </div>
                  )
                } else { //other peoples messages
                  return(
                    <div key={index} className={style.message}>
                      <div>{msg.content}</div>
                      <div className={style.user}>{msg.sender}</div>
                    </div>
                  )
                }
            })}
        </div>
    </div>
    <div>
        <input 
            placeholder="enter..."
            value = {newMessage}
            onChange={(e) => setNewMessage(e.target.value)}
        />
        <button onClick={sendMessage}>Send Message</button>
    </div>
    </>
  )
}

export default ChatRoomComponent