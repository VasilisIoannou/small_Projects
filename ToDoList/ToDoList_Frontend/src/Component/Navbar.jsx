import React from 'react'
import { useState } from 'react';
import Sidebar from './Sidebar';



const Navbar = ({createCard}) => {
  
    const [title,setTitle] = useState('');
    const [showSidebar,setShowSidebar] = useState(false);

  return (
    <div className="bg-white border-gray-200 dark:bg-gray-400"> 
        
        <div className ="flex gap-x-6 gap-y-2 mx-3 items-center p-2 ms-3">

          {showSidebar &&
          <aside id="default-sidebar" 
          className={`fixed top-0 left-0 z-40 w-64 h-screen transition-transform ${
            showSidebar ? 'translate-x-0' : '-translate-x-full'
          } sm:translate-x-0`}
          aria-label="Sidebar">
            <Sidebar createCard = {createCard}/>
          </aside> }

          <button data-drawer-target="default-sidebar" 
            onClick = {() => setShowSidebar(!showSidebar)}

            aria-controls="default-sidebar" 
            className="z-50 text-sm text-black rounded-lg hover:bg-gray-100 focus:outline-none focus:ring-2 focus:ring-gray-200 dark:text-gray-900 dark:hover:bg-gray-700 dark:focus:ring-gray-600">
            
            <svg className="w-6 h-6" 
              aria-hidden="true" 
              fill="currentColor" 
              viewBox="0 0 20 20" 
              xmlns="http://www.w3.org/2000/svg">
            <path clipRule="evenodd" 
            fillRule="evenodd" 
            d="M2 4.75A.75.75 0 012.75 4h14.5a.75.75 0 010 1.5H2.75A.75.75 0 012 4.75zm0 10.5a.75.75 0 01.75-.75h7.5a.75.75 0 010 1.5h-7.5a.75.75 0 01-.75-.75zM2 10a.75.75 0 01.75-.75h14.5a.75.75 0 010 1.5H2.75A.75.75 0 012 10z"></path> 
            </svg>
          </button>
         

          <input 
              className="z-50 tracking-widest text-black md:text-lg border-gray-200 dark:bg-gray-400 placeholder-gray-900 placeholder-opacity-50 outline-none" 
              id='title'
              name ='title'
              placeholder="Add your own Title"
              value = { title }
              onChange={(e)=> setTitle(e.target.value) }
          />
            

        </div>
    </div>
  )
}

export default Navbar;