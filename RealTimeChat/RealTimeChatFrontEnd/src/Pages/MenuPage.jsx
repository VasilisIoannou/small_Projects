import React, { useEffect, useState, useRef} from 'react'
import { useLocation } from 'react-router-dom'
import { useNavigate } from 'react-router-dom';

const MenuPage = () => {

    const location = useLocation();
    const { username } = location.state || {}
    const { password } = location.state || {}

    const [chatroomList,setChatroomList] = useState([]);

    const [chatroomName,setChatroomName] = useState(username+"'s chatroom");
    const [invitationCode,setInvitationCode] = useState("");

    const [joinCode,setJoinCode] = useState("");

    const [accountId,setAccoountId]= useState(null);
    const navigate = useNavigate();

    const [uniqueCode,setUniqueCode] = useState(false);

    const isInitialRender = useRef(true); 

    const UniqueInvitationCode = async()=>{
        try{
            const codeResult = await fetch( "http://localhost:8080/chatroom/room/get/code/"+invitationCode,{
                method: 'GET',
                headers: {"Content-Type":"application/json"}
            })

            const data = await codeResult.json();
            //console.log("data",data)
            
            const isUnique = data === null || (Array.isArray(data) && data.length === 0);
            //console.log("Is invitation code unique?", isUnique);

            if(!isUnique){
                alert("Sorry that code already exists");
                setInvitationCode("")
            }
    
            return isUnique;

        }catch (err) {
            console.error("Error checking unique code:", err);
            return false;
        }

    }

    useEffect(()=>{
        const RequestCreate = async() =>{
            try{
                console.log("create")
                const responce = await fetch("http://localhost:8080/chatroom/room/create/"+chatroomName+"@"+invitationCode+"@"+accountId,{
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
                setInvitationCode("");
                setChatroomName("");
    
            }catch(err){
                console.error("There was an error creating a chatroom", err);
                return null;
            }
        }

        if(uniqueCode === true){
            RequestCreate();
            setUniqueCode(false);
        }
    },[uniqueCode])

    const CreateChatroom = async ()=>{
        const isUnique = await UniqueInvitationCode();
        setUniqueCode(isUnique);
    }

    function GotoChatroom(title,chatroomCode,chatroomId){
        navigate(`/chatroom/${chatroomCode}`,{ state: { chatroomTitle : title , username: username, userId:accountId, chatroomId: chatroomId} })
    }

    const GoToAccountPage = ()=>{
        navigate(`/profile/${accountId}`,{ state: { profileId : accountId } })
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

    const ConnectAccountAndChatroom = async (chatroomId)=>{//Fix?
        try{
            const results = await fetch("http://localhost:8080/chatroom/account_chatroom/createConnection/"+accountId+"@"+chatroomId,{
                method:"PUT",
                headers:{"Content-Type":"application/json"}
            })
            if(!results.ok){
                throw new Error("Error with connect account and chatrooms")
            }
        }catch(err){
            console.error("Error in the ConnectAccountAndChatroom function",err)
        }
    }

    useEffect(()=>{
        const getAccount = async() => {
            const accountResult = await fetch("http://localhost:8080/account/get/"+username+"@"+password,{
                method: 'GET',
                headers: {"Content-Type":"application/json"}
            })

            const data = await accountResult.json();
            setAccoountId(data[0].id);
        }

        getAccount();
    },[])

    useEffect(()=>{
        if (!accountId || !isInitialRender.current) return; // Wait until accountId is available   
        const getExistingChatrooms = async()=>{
            try{
                const results = await fetch("http://localhost:8080/chatroom/account_chatroom/getChatroomsByAccount/"+accountId,{
                    method:'GET',
                    headers:{"Content-Type":"application/json"}
                })

                if(!results.ok){
                    throw new Error("Error with getiing existing classrooms ",accountId)
                }

                const data = await results.json();

                data.forEach(element => {
                    setChatroomList((prev)=>{
                        if(!prev.some((existingChatroom)=> (existingChatroom.id === element.id))){
                            return [...prev,element];
                        }
                        return prev;
                    });
                });

                isInitialRender.current = false;
            }catch(err){
                console.error("Error in the getExistingChatrooms function",err)
        }}

        if(isInitialRender.current){
            getExistingChatrooms();
        }
    },[accountId])

    const addAccountByCode = async (Code) =>{
        const results = fetch("http://localhost:8080/chatroom/account_chatroom/addAcountToChatroomByCode/"+Code+"@"+accountId,{
            method: 'PUT',
            headers: {"Content-Type":"application/json"}
        })

        if(!results.ok){
            throw new Error("Error with  connecting classrooms and the account by code");
        }
    }

    const EnterToNewChatroom = async(Code) => {
        addAccountByCode(Code)
        EnterChatroom(Code)
        setJoinCode("");
    }

    const DeleteChatroom = async(chatroomId)=>{
        try{
            const result = await fetch("http://localhost:8080/chatroom/chatroom/delete/"+chatroomId,{
                method:'DELETE',
                headers:{"Content-Type":"application/json"}
            })

            if(!result.ok){
                throw new Error("Error with deleting chatroom")
            }

            setChatroomList((prev)=> prev.filter(chatroom => chatroom.id !== chatroomId))
        }catch(err){
            console.error("Error in the DeleteCHatroom function",err)
        }
    }
  return (
    <>
    {username !== '' ? (//Make it pretty
        <>
        <div> Welcome {username} </div>
        <button onClick={()=>GoToAccountPage()}>Account Settings</button>
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
            <button onClick={()=>EnterToNewChatroom(joinCode)}>join Chat Room</button>
        </div>
        <div>Your Chatrooms</div>
        {chatroomList.map((chatroomInstance,id)=>{
            //Make a Card Component
            return(
            <div key={id}> 
               <div> Name: {chatroomInstance.name} </div> 
               <div> Code: {chatroomInstance.code} </div> 
               <button onClick={()=> GotoChatroom(chatroomInstance.name,chatroomInstance.code,chatroomInstance.id)}> Enter </button>
               <button onClick={()=>DeleteChatroom(chatroomInstance.id)}>Delete</button>
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