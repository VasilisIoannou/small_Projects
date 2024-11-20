import React, { useState } from 'react'
import { useNavigate } from 'react-router-dom';

const LogIn = () => {

  const [username,setUsername] = useState(""); 
  const [password,setPassword] = useState("");    
  const navigate = useNavigate();
 
  function moveToMainPage(){
    if (username.trim()) {
        navigate('/chatroomHook', { state: { username : username } }); 
    } else {
        alert("Please enter a username.");
    }
  }

  function requestCreate(){
    fetch('http://localhost:8080/account/create/'+username+'@'+password,{
      method: 'PUT',
      headers: {"Content-Type":"application/json"}
    }).then(responce => responce.json)
      .then()
  }

  function createUser(){
    



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
        <input 
            placeholder='password...' 
            value = {password}
            onChange={(e) => setPassword(e.target.value)}
        />
        <button onClick={createUser}> Enter</button>
    </div>
    </>
  )
}

export default LogIn