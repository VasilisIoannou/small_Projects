package com.Vasilis.RealTimeChat.accounts;

import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/account/")
@CrossOrigin(origins = "http://localhost:5173")
public class AccountController {
    private final AccountRepository accountRepository;
    public AccountController(AccountRepository accountRepository) {
        this.accountRepository = accountRepository;
    }

    @GetMapping("get/all")
    public List<AccountRecord> getAll(){
        return accountRepository.getAll();
    }

    @GetMapping("getByUsernamePassword/{username_password}") //This doesnt feel safe
    public List<AccountRecord> getByUsernameAndPassword(@PathVariable String username_password){
        String username = username_password.split("@")[0];
        String password = username_password.split("@")[1];
        return accountRepository.findByUsernameAndPassword(username,password);
    }
    @GetMapping("getById/{id}")
    public List<AccountRecord> getById(@PathVariable int id){
        return accountRepository.getAccountById(id);
    }

    @PutMapping("create/{username_password}")
    public Integer createAccount(@PathVariable String username_password){
        String username = username_password.split("@")[0];
        String password = username_password.split("@")[1];
        return accountRepository.createAccount(username,password);
    }
    @PutMapping("updateUsername/{accountId_newUsername}")
    public void updateUsername(@PathVariable String accountId_newUsername){
        Integer accountId = Integer.parseInt(accountId_newUsername.split("@")[0]);
        String newUsername = accountId_newUsername.split("@")[1];
        accountRepository.changeAccountName(accountId,newUsername);
    }

    @PutMapping("updatePassword/{accountId_newPassword}")
    public void updatePassword(@PathVariable String accountId_newPassword){
        Integer accountId = Integer.parseInt(accountId_newPassword.split("@")[0]);
        String newPassword = accountId_newPassword.split("@")[1];
        accountRepository.changeAccountPassword(accountId,newPassword);
    }
    @PutMapping("addFriend/{userId_friendId}")
    public void addFriend(@PathVariable String userId_friendId){
        Integer userId = Integer.parseInt(userId_friendId.split("@")[0]);
        Integer friendId = Integer.parseInt(userId_friendId.split("@")[1]);
        accountRepository.addAccountToFriends(userId,friendId);
    }

    @GetMapping("getFriends/{userId}")
    public List<AccountRecord> getFriends(@PathVariable String userId){
        Integer id = Integer.parseInt(userId);
        return  accountRepository.getFriends(id);
    }

    @GetMapping("getMutualFriends/{user1Id_user2Id}")
    public List<AccountRecord> getMutualFriends(@PathVariable String user1Id_user2Id){
        Integer user1id = Integer.parseInt(user1Id_user2Id.split("@")[0]);
        Integer user2Id = Integer.parseInt(user1Id_user2Id.split("@")[1]);
        return accountRepository.getMutualFriends(user1id,user2Id);
    }

    @DeleteMapping("delete/{id}")
    public void deleteAccount(@PathVariable int id){
        accountRepository.deleteAccount(id);
    }


}
