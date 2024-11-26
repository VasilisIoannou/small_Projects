import React from 'react'
import { StompSessionProvider } from 'react-stomp-hooks';
import { useLocation } from 'react-router-dom';
import ChatRoomComponent from './ChatRoomComponent';



const ChatRoomHook = () => {

  const location = useLocation();
  const {username = "not set"} = location.state || {}
  const {chatroomTitle = "not set"} = location.state || {}
  const {userId = "not set"} = location.state || {}
  const {chatroomId= "not set"} = location.state || {}

  return (
    <>
    <StompSessionProvider 
        url = {'http://localhost:8080/ws'} 
        onConnect={() => console.log("Connected to STOMP")}
        onDisconnect={() => console.log('Disconnected from STOMP')}
    >

        <ChatRoomComponent usernameLogIn = {username} chatroomTitle={chatroomTitle} userId={userId} chatroomId={chatroomId} />

    </StompSessionProvider>
    </>
  )
}

export default ChatRoomHook