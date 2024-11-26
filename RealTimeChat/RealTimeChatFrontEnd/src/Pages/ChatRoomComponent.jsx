import React, {useEffect, useState } from 'react'
import { useStompClient, useSubscription } from "react-stomp-hooks";

import style from '../Styles/ChatRoomComponent.module.css'


const ChatRoomComponent = ({usernameLogIn,userId,chatroomTitle,chatroomId}) => {

  //Get chatroomTitle from back end when is implemented

  const [messages, setMessages] = useState([]);
  const [newMessage,setNewMessage] = useState("");

  const stompClient = useStompClient(); 

  // const [userColors, setUserColors] = useState({}); 

  // const colors = [
  //   "#2763FF", "#A702FF", "#fdffb6", "#caffbf", "#9bf6ff", 
  //   "#a0c4ff", "#bdb2ff", "#ffc6ff", "#fffffc",
  // ]; // Define an array of colors to cycle through


  useSubscription("/topic/public",(message) => {
    const parseMessage = JSON.parse(message.body);
    setMessages((prev) => [...prev,parseMessage]);
  })

  function sendMessage(){
    if(stompClient && newMessage.trim() !==''){
        stompClient.publish({
            destination: '/app/chat.sendMessage',
            
            body: JSON.stringify({
                content: newMessage || "",
                sender: usernameLogIn || "Anonymous",
             })
        });

        SaveMessage(usernameLogIn,userId,chatroomId,newMessage);

        setNewMessage("");
    }
  }

  const SaveMessage = async (sender,senderId,chatroomId,body)=>{
    const encodedMessageContent = encodeURIComponent(body);
    try{
      const results = await fetch("http://localhost:8080/chatroom/messages/create/"+sender+"@"+encodedMessageContent+"@"+chatroomId+"@"+senderId ,{
        method:'PUT',
        headers:{"Content-Type":"application/json"}
      })

      if(!results.ok){
        throw new Error("Error with saving message");
      }
    }catch(err){
      console.error("Error in SaveMessage function ",err)
    }

  }

  useEffect(()=>{
    const GetExistingMessages = async()=>{
      try{
        const results = await fetch("http://localhost:8080/chatroom/messages/get/all",{//Fix when query is implemented in backend
          method:'GET',
          headers:{"Content-Type":"application/json"}
        })
        if(!results.ok){
          throw new Error("Error with getting message");
        }
        const data = await results.json();
        data.forEach(element => {
          setMessages((prev)=>{
              if(!prev.some((existingChatroom)=> (existingChatroom.id === element.id))){
                  return [...prev,element];
              }
              return prev;
          });
        });

      }catch(err){
        console.error("Error in GetExistingMessages function ",err)
      }
    }
    GetExistingMessages();
  },[])

  
  return (
    <>
    <div>{chatroomTitle}</div>
    <div>{usernameLogIn}</div>
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


    {/* {usernameLogIn !== '' && usernameLogIn  ? usernameLogIn : "Please Log In"} */}



  <div className={style.messageBox}>
    {messages.length > 0 ? (
      messages.map((msg, index) => {
        if (usernameLogIn === msg.sender) {
          // Your own messages
          return (
            <div key={index} className={style.messageLine}>
              <span className={style.message1}>{msg.content}</span>
              <br />
            </div>
          );
        } else {
          // Other people's messages
          return (
            <div key={index} className={style.messageLine}>
              <span className={style.message2}>{msg.content}</span>
              <br />
            </div>
          );
        }
      })
    ) : (
      <div className={style.noMessages}>No messages yet!</div>
    )}
  </div>

 

    </>
  )
}

export default ChatRoomComponent