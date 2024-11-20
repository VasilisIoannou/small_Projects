import React, { useState } from 'react'
import { useNavigate } from 'react-router-dom';
import style from '../Styles/LogIn.module.css'


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
    <div className = {style.loginPage}>
    <div className = {style.enterUsername}>Username</div>
    <div className = {style.inputContainer}>
        <input className = {style.usernameInput}
            placeholder='Enter a username...' 
            value = {username}
            onChange={(e) => setUsername(e.target.value)}
        />
        <button onClick={createUser} className = {style.usernameInputButton}> Enter</button>
    </div>
    </div>
  )
}

export default LogIn