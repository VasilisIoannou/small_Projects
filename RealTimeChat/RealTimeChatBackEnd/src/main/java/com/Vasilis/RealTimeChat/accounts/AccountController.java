package com.Vasilis.RealTimeChat.accounts;

import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/account/")
@CrossOrigin
public class AccountController {
    private final AccountRepository accountRepository;
    public AccountController(AccountRepository accountRepository) {
        this.accountRepository = accountRepository;
    }

    @GetMapping("get/all")
    public List<AccountRecord> getAll(){
        return accountRepository.getAll();
    }

    @GetMapping("get/{username_password}") //This doesnt feel safe
    public List<AccountRecord> getByUsernameAndPassword(@PathVariable String username_password){
        String username = username_password.split("@")[0];
        String password = username_password.split("@")[1];
        return accountRepository.findByUsernameAndPassword(username,password);
    }

    @PostMapping("create/{username_password}")
    public Integer createAccount(@PathVariable String username_password){
        String username = username_password.split("@")[0];
        String password = username_password.split("@")[1];
        return accountRepository.createAccount(username,password);
    }
}
