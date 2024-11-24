import React, { useState } from 'react'
import { useNavigate } from 'react-router-dom';
import style from '../Styles/LogIn.module.css'


const LogIn = () => {

  const [username,setUsername] = useState(""); 
  const [password,setPassword] = useState("");    

  const [loginUsername,setLoginUsername] = useState(""); 
  const [loginPassword,setLoginPassword] = useState("");  

  const navigate = useNavigate();
 
  function moveToMainPage(){
    if (username.trim()) {
        navigate('/chatroomHook', { state: { username : username } }); 
    } else {
        alert("Please enter a username!");
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

    console.log(data)

    if(data.length>0){
      moveToMainPage()
    } else {
      console.log("Account Doesn't Exist")
    }
  }

  function createUser(){
    requestCreate()
    moveToMainPage()
  }

  return (
    <div className ={style.loginPage}>
    <div id="signUp" className = {style.signUp}>
      <div>Sign Up</div>
      <div className = {style.things}>
          <input className ={style.usernameInput}
              placeholder='username...' 
              value = {username}
              onChange={(e) => setUsername(e.target.value)}
          />
          <input className ={style.usernameInput}
              placeholder='password...' 
              type = "password"
              value = {password}
              onChange={(e) => setPassword(e.target.value)}
          />
          <button className = {style.usernameInputButton} onClick={createUser}> Enter</button>
      </div>
    </div>

    <div id="logIn" className = {style.logIn}>
      <div>Log In</div>
      <div className = {style.things}>
          <input className ={style.usernameInput}
              placeholder='username...' 
              value = {loginUsername}
              onChange={(e) => setLoginUsername(e.target.value)}
          />
          <input className ={style.usernameInput}
              placeholder='password...' 
              type = "password"
              value = {loginPassword}
              onChange={(e) => setLoginPassword(e.target.value)}
          />
          <button className = {style.usernameInputButton} onClick={() => logInUser()}> Enter</button>
      </div>
      </div>
    </div>
  )
}

export default LogIn