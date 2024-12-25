import React, { useEffect } from 'react'
import { useLocation, useNavigate } from 'react-router-dom';
import { useState } from 'react';

const ProfilePage = () => {

    const location = useLocation();
    const {profileId} = location.state || {}
    const {yourProfileId} = location.state || {} //Add your profileId
    const {yourProfile} = location.state || false

    const navigate = useNavigate();

    const [profileName,setProfileName] = useState("");
    const [profilePassword,setProfilePassword] = useState("No passwordFetched");

    const [changeName,setChangeName] = useState(false);
    const [changePassword,setChangePassword] = useState(false);

    const [newName,setNewName] = useState(profileName);
    const [newPassword,setNewPassword] = useState(profilePassword);

    const [friendID,setFriendID] = useState("");
    const [friendList,setFriendList] = useState([]);

    
    useEffect(()=>{
        const FetchFriends = async ()=>{
            try{
                const results = await fetch("http://localhost:8080/account/getFriends/"+profileId,{
                    method:'GET',
                    headers:{"Content-Type":"application/json"}
                });
                if(!results.ok){
                    throw new Error("Failed fetching friends' data")
                }
    
                const friendData =  await results.json();
    
                setFriendList(friendData.map((currentFriend) => ({
                    id: currentFriend.id,
                    username: currentFriend.username
                })));
                
            }catch(err){
                console.error("There was an error in FetchAccountData",err);
                return null;
            } 
        }

        const FetchAccountData = async ()=>{
            try{
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
        //Add something to not Fetch Acount Password
            
        FetchFriends();
    },[])

    const UpdateUsername = async () =>{
        try{
            const result = await fetch("http://localhost:8080/account/updateUsername/"+profileId+"@"+newName,{
                method:'PUT',
                headers:{"Content-Type":"application/json"}
            });

            if(!result.ok){
                throw new Error("Failed updating profile username")
            }

            setChangeName(false)
            setProfileName(newName)
        }catch(err){
            console.error("There was an error in UpdateUsername",err);
            return null;
        } 
    }
    const UpdatePassword = async () =>{
        try{
            const result = await fetch("http://localhost:8080/account/updatePassword/"+profileId+"@"+newPassword,{
                method:'PUT',
                headers:{"Content-Type":"application/json"}
            });

            if(!result.ok){
                throw new Error("Failed updating profile password")
            }

            setChangePassword(false);
            setProfilePassword(newPassword);
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

            if(!result.ok){
                throw new Error("Failed deleting profile")
            }
        }catch(err){
            console.error("There was an error in DeleteProfile",err);
            return null;
        } 
    }

    const verifyDelete = () => {
        //Add a pop up which ask's for delete verification
        return true;
    }

    const DeleteAccount = async()=>{
        //Ask for verification
        if(verifyDelete){
            DeleteAccountRequest();
            navigate('/login');
        }
    }

    const fetchFriendData = async (friendId)=>{
        try{
            const res = await fetch("http://localhost:8080/account/getById/"+friendId,{
                method:'GET',
                headers:{"Content-Type":"application/json"}
            });

            if(!res.ok){
                throw new Error("There was an error in fetching the friends data");
            }

            const data = await res.json();
            return data;
        } catch {
            console.error("There was an erro in the fetchFriendData function");
            return null;
        }
    }

    const addFriend = async() =>{
        try{
            const result = await fetch("http://localhost:8080/account/addFriend/"+profileId+"@"+friendID,{
                method:'PUT',
                headers:{"Content-Type":"application/json"}
            });

            if(!result.ok){
                throw new Error("Failed deleting profile")
            }

            //Fetch Friend Data from backend
            const friendData = await fetchFriendData(friendID);


            const newFriend = {//Fix
                id: friendID,
                username: friendData[0].username
            }

            setFriendList((prev) => [...prev,newFriend]);
            setFriendID("");    
        }catch(err){
            console.error("There was an error in adding Friend",err);
            return null;
        } 
    }

     const gotoMenuPage = () =>{
        navigate('/menu', { state: {accountNavId: yourProfileId } }); 
        
     }
     const gotoAccountPage = (FriendProfileID,FriendUsername) => {
        navigate(`/profile/${FriendProfileID}`, { state: { profileId : FriendProfileID, yourProfileId: yourProfileId ,yourProfile: false} }); 
     }
  return (
    <>
        {yourProfile ? (
        <>
        <button onClick={() => gotoMenuPage()}>Go to Main Menu</button>

        <p>Add a profile Picture</p>

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
            <div>About me:</div>
            <p>Make an About Me Section</p>
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
            <p>Your Friend List:</p>
            {friendList.map((friend) => {
                return(
                    <div key={friend.id}>
                        {friend.username}
                        <button onClick={()=>gotoAccountPage( friend.id,friend.username )}> go to profile account</button>
                    </div>
                )
            })}
        <div>

        </div>

        <div>
            <p>Add a new Friend!</p>
            <input 
                placeholder="Enter your friend's ID"
                value={friendID}
                onChange={(e)=>setFriendID(e.target.value)}
            />
            <button onClick={()=>addFriend()}>Add</button>
        </div>

        <div>
            <div>More Settings</div>
            <button onClick={DeleteAccount}>Delete Account</button>
        </div>
        </>
  ) : (
    <>
    <button onClick={() => gotoMenuPage()}>Go Back</button>
    
    <div> Name: {profileName} </div>
    <p>Your Mutual Friend List:</p>
        
    {/* add mutual friend List
    If not already Friends Add a add friend */}

    <div>Your are Friends! </div>
    <button> Add Friend </button>
    </>
  )}
  </>
)}

export default ProfilePage