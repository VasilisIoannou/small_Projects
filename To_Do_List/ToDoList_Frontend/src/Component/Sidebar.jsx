import React, { useState } from 'react'
import{SidebarData} from './SidebarData'
import { MdAddCard } from "react-icons/md"; 
import { FaSearch } from "react-icons/fa";
import { useNavigate } from 'react-router-dom';

const Sidebar = ({createCard}) => {

    const [search , setSearch] = useState('');
    const navigate = useNavigate();

    function followLink(link){
        if(link!=="null"){
            window.location.pathname = link;
        }
    }

    function optionFunction(type,link){
        if(type === "CreateCard"){
            console.log(createCard);
            
        }
        if(link!=="null"){
            followLink(link)
        }
    }

    function searchByTitle(title){
        fetch('http://localhost:8080/api/content/title/'+title,{
            method: 'GET',
            headers: {"Content-Type": "application/json" },
          }).then(response => response.json())
          .then(data => {
            console.log(data);
            navigate('/search', { state: { searchData: data } });
          })
          .catch(error => {
              console.error('Error:', error);  // Handle errors
          });
    }
    function searchByBody(body){
        fetch('http://localhost:8080/api/content/body/'+body,{
            method: 'GET',
            headers: {"Content-Type": "application/json" },
          }).then(response => response.json())
          .then(data => {
            console.log(data);
            navigate('/search', { state: { searchData: data } });
          })
          .catch(error => {
              console.error('Error:', error);  // Handle errors
          });
    }

    function searchByTitleKeyPress(event,body){
        if(event.key === 'Enter') {
            searchByBody(body);
        }
    }
  return (
    <div className ="fixed top-11 left-0 z-40 w-64 h-screen transition-transform -translate-x-full sm:translate-x-0">
        <div className="h-full px-3 py-16 overflow-y-auto bg-gray-50 dark:bg-gray-300">
        <ul className="space-y-8 text-lg font-medium">

            <button  className = 'flex items-center space-x-2'
            onClick={()=> {{ if(window.location.pathname === "/home") { createCard() }}}}> <MdAddCard className = {window.location.pathname !== "/home" ? 'opacity-50' : ''} /><div className = {window.location.pathname !== "/home" ? 'opacity-50' : ''}>Create new Card</div> </button>
        
        {SidebarData.map((val, key) => {
            return(
                <li key={key} className="flex items-center space-x-2" onClick={() => optionFunction(val.type,val.link)}> 
                    <div>{val.icon}</div>{" "}
                    <div>{val.title}</div>{" "}
                </li>
            );
        })}
        <div  className="flex items-center space-x-2">  <FaSearch /> 
            <input placeholder="Search"
                className="text-black text-lg md:text-lg border-gray-200 dark:bg-gray-300 placeholder-gray-900 placeholder-opacity-50 outline-none" 
                value = { search }
                onKeyDown={(e) => searchByTitleKeyPress(e , search)}
                onChange={(e)=> setSearch(e.target.value) } />
        </div>
        </ul>
        </div>
    </div>
  )
}

export default Sidebar

