import React from 'react'
import { RouterProvider, Route, createBrowserRouter, createRoutesFromElements } from 'react-router-dom'

import MainLayout from './layouts/MainLayout'
import HomePage from './Pages/HomePage'
import CalendarPage from './Pages/CalendarPage'
import PageNotFound from './Pages/PageNotFound'
import SearchPage from './Pages/SearchPage'

const router = createBrowserRouter(
  createRoutesFromElements(
    <Route element={<MainLayout />} >
      <Route index path ='/home' element={<HomePage />} />
      <Route index path='/calendar' element={<CalendarPage />} />
      <Route index path='/search' element={<SearchPage />} />
      <Route path='*' index element={<PageNotFound />} />
    </Route>
  )
)

const App = () => {
  return (
    <RouterProvider  router={router} />
  )
}

export default App