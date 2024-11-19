import React from 'react'
import { useLocation } from 'react-router-dom'

const MainPage = () => {

  const location = useLocation();
  const { username } = location.state || {}


  return (
    <>
    {username !== '' ? (
        <div> Welcome {username} </div>
    ) : (
       <div> "Please Log In" </div>
    )}

    <div>
        <button >Create Chat Room</button>
    </div>
    <div>
        <input placeholder='invitation code' />
        <button>join Chat Room</button>
    </div>
    </>
  )
}

export default MainPage