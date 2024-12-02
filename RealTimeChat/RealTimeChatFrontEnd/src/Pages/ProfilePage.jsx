import React, { useEffect } from 'react'
import { useLocation } from 'react-router-dom';
import { useState } from 'react';

const ProfilePage = () => {

    const location = useLocation();
    const {profileId} = location.state || {}

    const [profileName,setProfileName] = useState("no name fetched");
    const [profilePassword,setProfilePassword] = useState("No passwordFetched");

    
    useEffect(()=>{
        const FetchAccountData = ()=>{
            const results = fetch("http://localhost:8080/account/get/"+profileId,{
                
            })
        }
    },[])

    const UpdateUsername = async (newUsername) =>{
        try{
            const result = await fetch("http://localhost:8080/account/updateUsername/"+profileName+"@"+newUsername,{
                method:'PUT',
                headers:{"Content-Type":"application/json"}
            });

            if(!responce.ok){
                throw new Error("Failed updating profile username")
            }
        }catch(err){
            console.error("There was an error in UpdateUsername",err);
            return null;
        } 
    }
    const UpdatePassword = async (newPassword) =>{
        try{
            const result = await fetch("http://localhost:8080/account/updatePassword/"+profileName+"@"+newPassword,{
                method:'PUT',
                headers:{"Content-Type":"application/json"}
            });

            if(!responce.ok){
                throw new Error("Failed updating profile password")
            }
        }catch(err){
            console.error("There was an error in UpdatePassword",err);
            return null;
        } 
    }
    const DeleteAccount = async () =>{
        try{
            const result = await fetch("http://localhost:8080/chatroom/chatroom/delete/"+profileId,{
                method:'PUT',
                headers:{"Content-Type":"application/json"}
            });

            if(!responce.ok){
                throw new Error("Failed deleting profile")
            }
        }catch(err){
            console.error("There was an error in DeleteProfile",err);
            return null;
        } 
    }
  return (
    <>
        <div>
            <div> Name: {profileName} </div>
            <button>Change</button>
        </div>
        <div>
            <div> Password: {profilePassword} </div>
            <button>Change</button>
        </div>
        <div>
            <div>More Settings</div>
            <button>Delete Account</button>
        </div>
    </>
  )
}

export default ProfilePage