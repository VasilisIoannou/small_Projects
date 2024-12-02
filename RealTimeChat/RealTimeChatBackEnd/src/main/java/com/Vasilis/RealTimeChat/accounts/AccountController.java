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

    @PostMapping("create/{username_password}")
    public Integer createAccount(@PathVariable String username_password){
        String username = username_password.split("@")[0];
        String password = username_password.split("@")[1];
        return accountRepository.createAccount(username,password);
    }
    @PostMapping("updateUsername/{accountId_newUsername}")
    public void updateUsername(@PathVariable String accountId_newUsername){
        Integer accountId = Integer.parseInt(accountId_newUsername.split("@")[0]);
        String newUsername = accountId_newUsername.split("@")[1];
        accountRepository.changeAccountName(accountId,newUsername);
    }

    @PostMapping("updatePassword/{accountId_newPassword}")
    public void updatePassword(@PathVariable String accountId_newPassword){
        Integer accountId = Integer.parseInt(accountId_newPassword.split("@")[0]);
        String newPassword = accountId_newPassword.split("@")[1];
        accountRepository.changeAccountName(accountId,newPassword);
    }

    @DeleteMapping("delete/{id}")
    public void deleteAccount(@PathVariable int id){
        accountRepository.deleteAccount(id);
    }

}
