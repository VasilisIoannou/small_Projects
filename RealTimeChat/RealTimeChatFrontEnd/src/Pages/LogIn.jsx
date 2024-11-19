import React, { useState } from 'react'
import { useNavigate } from 'react-router-dom';

const LogIn = () => {

  const [username,setUsername] = useState("");   
  const navigate = useNavigate();
 
  function moveToMainPage(){
    if (username.trim()) {
        navigate('/chatroomHook', { state: { username : username } }); 
    } else {
        alert("Please enter a username.");
    }
  }
  function createUser(){
    //connect with back end
    moveToMainPage()
  }

  return (
    <>
    <div>Enter Username</div>
    <div>
        <input 
            placeholder='username...' 
            value = {username}
            onChange={(e) => setUsername(e.target.value)}
        />
        <button onClick={createUser}> Enter</button>
    </div>
    </>
  )
}

export default LogIn