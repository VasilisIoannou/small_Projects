import React, { useState } from 'react'
import { useNavigate } from 'react-router-dom';
import style from '../Styles/LogIn.module.css'


const LogIn = () => {

  const [username,setUsername] = useState(""); 
  const [password,setPassword] = useState("");    

  const [loginUsername,setLoginUsername] = useState(""); 
  const [loginPassword,setLoginPassword] = useState("");  

  const navigate = useNavigate();
 
  function moveToMenuPage(usernamePass){
    if (usernamePass.trim()) {
        navigate('/menu', { state: { username : usernamePass } }); 
    } else {
        alert("Please enter a username!");
    }
  }

  function requestCreate(){
    const encodedUsername = encodeURIComponent(username);
    const encodedPassword = encodeURIComponent(password)
    fetch('http://localhost:8080/account/create/'+encodedUsername+'@'+encodedPassword,{
      method: 'POST',
      headers: {"Content-Type":"application/json"}
    }).then(responce => {
      if(!responce.ok){
        throw new Error(`Failed to create account`);
      }
      return responce.json()
    }).catch(err => {
      console.error("Failed to create card:", err);
    })
  }

  const logInUser = async() =>{
    const res = await fetch('http://localhost:8080/account/get/'+loginUsername+'@'+loginPassword);
    const data = await res.json();

    if(data.length > 0){
      moveToMenuPage(loginUsername)
    } else {
      console.log("Account Doesn't Exist")
    }
  }

  function createUser(){
    //Dont allow duplicate accounts
    requestCreate()
    moveToMenuPage(username)
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