import React, { useEffect, useState } from 'react'
import { useLocation } from 'react-router-dom'
import { useNavigate } from 'react-router-dom';

const MenuPage = () => {

    const location = useLocation();
    const { username } = location.state || {}

    const [chatroomList,setChatroomList] = useState([]);

    const [accountID,setAccountID] = useState(1);//Fix when bakc end is implemented

    const [chatroomName,setChatroomName] = useState(username+"'s chatroom");
    const [invitationCode,setInvitationCode] = useState("");

    const [joinCode,setJoinCode] = useState("");

    const navigate = useNavigate();

    const CreateChatroom = async()=>{
        try{
            const responce = await fetch("http://localhost:8080/chatroom/room/create/"+chatroomName+"@"+invitationCode+"@"+accountID,{
                method: 'PUT',
                headers: {"Content-Type":"application/json"}
            });

            if(!responce.ok){
                throw new Error("Failed to create chatroom")
            }

            const data = await responce.json();
            const newChatroom = {
                id: data,
                name: chatroomName,
                code: invitationCode
            }
            setChatroomList((prev) => [...prev,newChatroom])

            ConnectAccountAndChatroom(data);

        }catch(err){
            console.error("There was an error creating a chatroom", err);
            return null;
        }
    }

    function GotoChatroom(title,chatroomCode,chatroomId){
        //Make other route for each chatroom
        console.log(title)
        navigate(`/chatroom/${chatroomCode}`,{ state: { chatroomTitle : title , username: username, userId:accountID, chatroomId: chatroomId} })
    }


    const getChatroom = async(code)=>{   
        try{
            const responce = await fetch("http://localhost:8080/chatroom/room/get/code/"+code ,{
                method:'GET',
                headers:{"Content-Type":"application/json"}
            });

            if(!responce.ok){
                throw new Error("Failed getting a chatroom")
            }

            const data = await responce.json();
            return data;
        }catch(err){
            console.error("There was an error with getting the chatroom",err);
            return null;
        } 
    }

    async function CheckCode (code){
        try{
            const currentChatroom = await getChatroom(code);
            console.log(currentChatroom[0].code)
            if(currentChatroom[0]?.code===code){
                return currentChatroom[0];
            }
            return null;
        }catch(err){
            console.error("Error in checkCode",err);
            return null;
        }
        
    }
    const EnterChatroom = async(code) =>{
        const currentChatroom = await CheckCode(code);
        console.log(currentChatroom);
        if(currentChatroom !== null){
            GotoChatroom(currentChatroom.name,currentChatroom.code,currentChatroom.id);
        }else{
            console.log("The chatroom doesnt exists");
        }
        
    }
    const ConnectAccountAndChatroom = async (chatroomId)=>{
        try{
            const results = fetch("http://localhost:8080/chatroom/account_chatroom/createConnection/"+accountID+"@"+1+chatroomId,{
                method:"PUT",
                headers:{"Content-Type":"application/json"}
            })
            if(!results.ok){
                throw new Error("Error with getiing existing classrooms")
            }
        }catch(err){
            console.error("Error in the getExistingChatrooms function",err)
        }
    }

    //fetch chatrrom data from back end
    useEffect(()=>{
        const getExistingChatrooms = async()=>{
            try{
                const results = await fetch("http://localhost:8080/chatroom/room/get/all",{
                    method:'GET',
                    headers:{"Content-Type":"application/json"}
                })
                if(!results.ok){
                    throw new Error("Error with getiing existing classrooms")
                }

                const data = await results.json();

                data.forEach(element => {
                    setChatroomList((prev)=>{
                        if(!prev.some((existingChatroom)=> (existingChatroom.id === element.id))){//When implemented in the backend Add unique to each account
                            return [...prev,element];
                        }
                        return prev;
                    });
                });
            }catch(err){
                console.error("Error in the getExistingChatrooms function",err)
            }}

        getExistingChatrooms();
    },[])

  return (
    <>
    {username !== '' ? (//Make it pretty
        <>
        <div> Welcome {username} </div>

        <div>
            <input 
                placeholder="Chatroom's name"
                value = {chatroomName}
                onChange={(e) => setChatroomName(e.target.value)}
            />
             <input 
                placeholder="Chatroom's invitation code"
                type = "password"
                value = {invitationCode}
                onChange={(e) => setInvitationCode(e.target.value)}
            />
            <button onClick={CreateChatroom}>Create Chat Room</button>
        </div>
        <div>
            <input 
                placeholder='invitation code' 
                value = {joinCode}
                onChange={(e)=> setJoinCode(e.target.value)}
                type = "password" 
            />
            <button onClick={()=>EnterChatroom(joinCode)}>join Chat Room</button>
        </div>
        <div>Your Chatrooms</div>
        {chatroomList.map((chatroomInstance,id)=>{
            //Make a Card Component
            return(
            <div key={id}> 
               <div> Name: {chatroomInstance.name} </div> 
               <div> Code: {chatroomInstance.code} </div> 
               <button onClick={()=> GotoChatroom(chatroomInstance.name,chatroomInstance.code,chatroomInstance.id)}> Enter </button>
            </div>
            )
        })}
        </>
    ) : (//Make it pretty
        <> 
        <div> "Please Log In" </div>
        <button> Go to Log In Page </button>
        </>
    )}
    </>
  )
}

export default MenuPage