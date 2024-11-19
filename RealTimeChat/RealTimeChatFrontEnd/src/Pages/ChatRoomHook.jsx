import React from 'react'
import { StompSessionProvider } from 'react-stomp-hooks';
import { useLocation } from 'react-router-dom';
import ChatRoomComponent from './ChatRoomComponent';



const ChatRoomHook = () => {

  const location = useLocation();
  const {username} = location.state || {}


  return (
    <>
    <StompSessionProvider 
        url = {'http://localhost:8080/ws'} 
        onConnect={() => console.log("Connected to STOMP")}
        onDisconnect={() => console.log('Disconnected from STOMP')}
    >

        <ChatRoomComponent usernameLogIn = {username} />

    </StompSessionProvider>
    </>
  )
}

export default ChatRoomHook