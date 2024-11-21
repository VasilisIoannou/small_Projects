import React, {useState } from 'react'
import { useStompClient, useSubscription } from "react-stomp-hooks";

import style from '../Styles/ChatRoomComponent.module.css'


const ChatRoomComponent = ({usernameLogIn}) => {

  const [messages, setMessages] = useState([]);
  const [newMessage,setNewMessage] = useState("");

  const stompClient = useStompClient(); 
  const [userColors, setUserColors] = useState({}); 

  const colors = [
    "#2763FF", "#A702FF", "#fdffb6", "#caffbf", "#9bf6ff", 
    "#a0c4ff", "#bdb2ff", "#ffc6ff", "#fffffc",
  ]; // Define an array of colors to cycle through


  useSubscription("/topic/public",(message) => {
    const parseMessage = JSON.parse(message.body);

    setUserColors((prevColors) => {
      if (!prevColors[parseMessage.sender]) {
        const nextColor = colors[Object.keys(prevColors).length % colors.length];
        return { ...prevColors, [parseMessage.sender]: nextColor };
      }
      return prevColors;
    });

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
    <div className = {style.theInput}>
        <input className = {style.messageInput}
            placeholder ="Enter new message"
            onKeyDown = {(e) => {
              if(e.key === 'Enter' ){
                sendMessage()
              }}
              
            }
            value = {newMessage}
            onChange={(e) => setNewMessage(e.target.value)}
        />
        {/* <button className = {style.MessageButton} onClick={sendMessage}>Send</button><br/><br/> */}
    </div>

    <div className ={style.MessageBox}>
        {usernameLogIn !== '' && usernameLogIn  ? usernameLogIn : "Please Log In"}
        <div className={style.messageBox}>
            {messages.map((msg,index)=>{
                if(usernameLogIn === msg.sender){ //Your own messages
                  return(
                    <div key={index} className={style.message}>d
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

    </>
  )
}

export default ChatRoomComponent