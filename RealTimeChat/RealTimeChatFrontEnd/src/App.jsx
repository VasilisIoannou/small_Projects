import React from 'react'
import { createBrowserRouter, createRoutesFromElements, Route,RouterProvider } from 'react-router-dom'

import MainPage from './Pages/MainPage'
import LogIn from './Pages/LogIn'
import Custom404Page from './Pages/Custom404Page'
import MainLayout from './Layout/MainLayout'
import ChatRoomHook from './Pages/ChatRoomHook'



const App = () => {

  const router = createBrowserRouter(
    createRoutesFromElements(
      <Route element ={<MainLayout />}>
        <Route index path ="/login" element={<LogIn />} />
        <Route index path="/main" element={<MainPage />} />
        <Route index path="/chatroomHook" element ={<ChatRoomHook />}/>
        <Route path="*" element={<Custom404Page />} />
      </Route>
    )
  )

  return (
    <RouterProvider router={router} />
  )
}

export default App