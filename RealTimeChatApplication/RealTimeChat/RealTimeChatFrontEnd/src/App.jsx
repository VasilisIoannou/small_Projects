import React from 'react'
import { createBrowserRouter, createRoutesFromElements, Route,RouterProvider } from 'react-router-dom'

import MenuPage from './Pages/MenuPage'
import LogIn from './Pages/LogIn'
import Custom404Page from './Pages/Custom404Page'
import MainLayout from './Layout/MainLayout'
import ChatRoomHook from './Pages/ChatRoomHook'
import ProfilePage from './Pages/ProfilePage'


const App = () => {

  const router = createBrowserRouter(
    createRoutesFromElements(
      <Route element ={<MainLayout />}>
        <Route index path ="/login" element={<LogIn />} />
        <Route index path="/menu" element={<MenuPage />} />
        <Route index path="/chatroom/:chatroomId" element ={<ChatRoomHook />}/>
        <Route index path="/profile/:profileId" element = {<ProfilePage />}/>
        <Route path="*" element={<Custom404Page />} />
      </Route>
    )
  )

  return (
    <RouterProvider router={router} />
  )
}

export default App