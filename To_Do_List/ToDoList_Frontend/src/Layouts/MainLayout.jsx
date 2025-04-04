import React from 'react'
import { Outlet } from 'react-router-dom'
import Navbar from '../Component/Navbar.jsx'
import { useState } from 'react'


//Write a read me file

const MainLayout = () => {

  const [createCard, setCreateCard] = useState(null);

  return (
    <div  className="h-full min-h-screen bg-slate-800"> 
      <Navbar createCard={createCard} />
      <Outlet context={{ setCreateCard }}/>
    </div>

  )
}

export default MainLayout