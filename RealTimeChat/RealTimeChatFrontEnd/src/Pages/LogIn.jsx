import React, { useState } from 'react'
import { useNavigate } from 'react-router-dom';
import style from '../Styles/LogIn.module.css'


const LogIn = () => {

  const [username,setUsername] = useState(""); 
  const [password,setPassword] = useState("");    

  const [loginUsername,setLoginUsername] = useState(""); 
  const [loginPassword,setLoginPassword] = useState("");  

  const navigate = useNavigate();
 
  function moveToMainPage(usernamePass){
    if (usernamePass.trim()) {
        navigate('/chatroomHook', { state: { username : usernamePass } }); 
    } else {
        alert("Please enter a username.");
    }
  }

  function requestCreate(){
    fetch('http://localhost:8080/account/create/'+username+'@'+password,{
      method: 'POST',
      headers: {"Content-Type":"application/json"}
    }).then(responce => responce.json)
    .catch(err => {
      console.error("Failed to create card:", err);
    })
  }

  const logInUser = async() =>{
    const res = await fetch('http://localhost:8080/account/get/'+loginUsername+'@'+loginPassword);
    const data = await res.json();

    if(data.length > 0){
      moveToMainPage(loginUsername)
    } else {
      console.log("Account Doesnt Exists")
    }
  }

  function createUser(){
    //Dont allow duplicate accounts
    requestCreate()
    moveToMainPage(username)
  }

  return (
    <>
    <div id="signUp">
      <div>Sign Up</div>
      <div>
          <input 
              placeholder='username...' 
              value = {username}
              onChange={(e) => setUsername(e.target.value)}
          />
          <input 
              placeholder='password...' 
              type = "password"
              value = {password}
              onChange={(e) => setPassword(e.target.value)}
          />
          <button onClick={createUser}> Enter</button>
      </div>
    </div>

    <div id="logIn">
      <div>Log In</div>
      <div>
          <input 
              placeholder='username...' 
              value = {loginUsername}
              onChange={(e) => setLoginUsername(e.target.value)}
          />
          <input 
              placeholder='password...' 
              type = "password"
              value = {loginPassword}
              onChange={(e) => setLoginPassword(e.target.value)}
          />
          <button onClick={() => logInUser()}> Enter</button>
      </div>
    </div>
    </>
  )
}

export default LogIn