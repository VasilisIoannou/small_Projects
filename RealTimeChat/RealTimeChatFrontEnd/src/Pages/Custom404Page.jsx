import React from 'react'
import styles from '../Styles/Custom404Style.module.css'
import'../index.css'

const Custom404Page = () => {
  return (
    <div className={styles.mainDiv}>
        <div>Sorry Page Not Found!</div>
        <div><a href="/login" className={styles.linkButton}> Go back to Main Page </a></div>
    </div>
  );
  
}

export default Custom404Page