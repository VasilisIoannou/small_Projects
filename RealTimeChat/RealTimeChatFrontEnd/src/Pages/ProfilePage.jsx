import React from 'react'
import { useLocation } from 'react-router-dom';
import { useState } from 'react';

const ProfilePage = () => {

    const location = useLocation();
    const {profileId} = location.state || {}

    const [profileName,setProfileName] = useState("no name fetched");
    const [profilePassword,setProfilePassword] = useState("No passwordFetched");

  return (
    <>
        <div>
            <div> Name: {profileName} </div>
            <button>Change</button>
        </div>
        <div>
            <div> Password: {profilePassword} </div>
            <button>Change</button>
        </div>
        <div>
            <div>More Settings</div>
            <button>Delete Account</button>
        </div>
    </>
  )
}

export default ProfilePage