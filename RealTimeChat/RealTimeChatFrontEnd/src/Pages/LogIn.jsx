import React, { useState } from 'react'
import { useNavigate } from 'react-router-dom';
import style from '../Styles/LogIn.module.css'


const LogIn = () => {

  const [username,setUsername] = useState(""); 
  const [password,setPassword] = useState("");    

  const [loginUsername,setLoginUsername] = useState(""); 
  const [loginPassword,setLoginPassword] = useState("");  

  const navigate = useNavigate();
 
  function moveToMenuPage(accountId){
    if (accountId > 0) {
        navigate('/menu', { state: { accountNavId: accountId} }); 
    } else {
        alert("Please enter a username!");
    }
  }

  const requestCreate = async()=>{
    const encodedUsername = encodeURIComponent(username);
    const encodedPassword = encodeURIComponent(password);
    try{
      const responce = await fetch('http://localhost:8080/account/create/'+encodedUsername+'@'+encodedPassword,{//Check username Uniqueness
        method: 'PUT',
        headers: {"Content-Type":"application/json"}
      })

      if(!responce.ok){
        throw new Error(`Failed to create account`);
      }

      const accountId = await responce.json();
      return accountId;
    }catch(err){
      console.error("Failed to create card:", err);
    }
  }

  const logInUser = async() =>{ //Fix
    const encodedUsername = encodeURIComponent(loginUsername);
    const encodedPassword = encodeURIComponent(loginPassword);
    const res = await fetch('http://localhost:8080/account/getByUsernamePassword/'+encodedUsername+'@'+encodedPassword,{
      method: 'GET',
      headers: {"Content-Type":"application/json"}
    });
    const data = await res.json();
    console.log(data)

    if(!res.ok){
      throw new Error('Failed to Log In user')
    }

    if(data.length > 0){
      //moveToMenuPage(loginUsername,loginPassword)
    } else {
      console.log("Account Doesn't Exist")
    }
  }

  const createUser = async () => {//Dont allow duplicate accounts
    try {
      const newId = await requestCreate();
      moveToMenuPage(newId);
    } catch (error) {
      console.error("Error creating user:", error);
    }
  };

  return (
    <div className ={style.loginPage}>
    <div id="signUp" className = {style.signUp}>
      <div>Sign Up</div>
      <div className = {style.things}>
          <input className ={style.usernameInput}
              placeholder='Enter a username' 
              value = {username}
              onChange={(e) => setUsername(e.target.value)}
          />
          <input className ={style.usernameInput}
              placeholder='Enter a password' 
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
              placeholder='Enter your username' 
              value = {loginUsername}
              onChange={(e) => setLoginUsername(e.target.value)}
          />
          <input className ={style.usernameInput}
              placeholder='Enter your password' 
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