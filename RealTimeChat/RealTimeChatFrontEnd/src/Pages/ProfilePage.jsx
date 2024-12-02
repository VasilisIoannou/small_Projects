import React, { useEffect } from 'react'
import { useLocation } from 'react-router-dom';
import { useState } from 'react';

const ProfilePage = () => {

    const location = useLocation();
    const {profileId} = location.state || {}

    const [profileName,setProfileName] = useState("no name fetched");
    const [profilePassword,setProfilePassword] = useState("No passwordFetched");

    const [changeName,setChangeName] = useState(false);
    const [changePassword,setChangePassword] = useState(false);

    const [newName,setNewName] = useState(profileName);
    const [newPassword,setNewPassword] = useState(profilePassword);
    
    useEffect(()=>{
            const FetchAccountData = async ()=>{
                try{
                    console.log("trying: http://localhost:8080/account/getById/"+profileId)
                    const results = await fetch("http://localhost:8080/account/getById/"+profileId,{
                        method:'GET',
                        headers:{"Content-Type":"application/json"}
                    });
                    if(!results.ok){
                        throw new Error("Failed fetching profile data")
                    }

                    const data =  await results.json();
                    setProfileName(data[0].username);
                    setNewName(data[0].username)
                    setProfilePassword(data[0].password);
                    setNewPassword(data[0].password);
                }catch(err){
                    console.error("There was an error in FetchAccountData",err);
                    return null;
                } 
            }
            FetchAccountData();
    },[])

    const UpdateUsername = async () =>{
        try{
            const result = await fetch("http://localhost:8080/account/updateUsername/"+profileName+"@"+newName,{//Fix Cross Origins
                method:'PUT',
                headers:{"Content-Type":"application/json"}
            });

            if(!responce.ok){
                throw new Error("Failed updating profile username")
            }

            setChangeName(false)
        }catch(err){
            console.error("There was an error in UpdateUsername",err);
            return null;
        } 
    }
    const UpdatePassword = async () =>{
        try{
            const result = await fetch("http://localhost:8080/account/updatePassword/"+profileName+"@"+newPassword,{
                method:'PUT',
                headers:{"Content-Type":"application/json"}
            });

            if(!responce.ok){
                throw new Error("Failed updating profile password")
            }

            setChangePassword(false);
        }catch(err){
            console.error("There was an error in UpdatePassword",err);
            return null;
        } 
    }
    const DeleteAccountRequest = async () =>{
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

    const DeleteAccount = async()=>{
        //Ask for verification
        if(verifyDelete){
            DeleteAccountRequest();
            //navigate to logIn Page
        }
    }
  return (
    <>
        <div>
            {!changeName ?(
                <>
                <div> Name: {profileName} </div>
                <button onClick={()=>setChangeName(!changeName)}>Change</button>
                </>
            ):(
                <>
                    <div> Name </div>
                    <input 
                        placeholder="Enter new username"
                        value={newName}
                        onChange={(e)=>setNewName(e.target.value)}
                    />
                    <button onClick={()=>UpdateUsername()}>Keep Change</button>
                    <button onClick={()=>setChangeName(!changeName)}>Keep Old Name</button>
                </>
            )}
            
        </div>

        <div>
            {!changePassword ?(
                <>
                <div> Password: {profilePassword} </div>
                <button onClick={()=>setChangePassword(!changePassword)}>Change</button>
                </>
            ):(
                <>
                    <div> Name </div>
                    <input 
                        placeholder="Enter new password"
                        value={newPassword}
                        onChange={(e)=>setNewPassword(e.target.value)}
                    />
                    <button onClick={()=>UpdatePassword()}>Keep Change</button>
                    <button onClick={()=>setChangePassword(!changePassword)}>Keep Old Password</button>
                </>
            )}
            
        </div>

        <div>
            <div>More Settings</div>
            <button onClick={DeleteAccount}>Delete Account</button>
        </div>
    </>
  )
}

export default ProfilePage