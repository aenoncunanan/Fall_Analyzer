package dlsu;

import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.*;

import java.io.*;
import java.net.URL;
import java.util.*;

public class exploreController implements Initializable{

    public Label usernameHello;
    public TextField usernameLogin;
    public PasswordField passwordLogin;
    public PasswordField confirmLogin;
    public Label feedbackLabelLogin;
    public Button editLogin;
    public Button saveLogin;

    public TextField firstNameProfile;
    public TextField LastNameProfile;
    public ChoiceBox genderProfile;
    public TextField ageProfile;
    public TextField addressProfile;
    public TextField contactNumberProfile;
    public Label feedbackLabel;
    public Button editProfile;
    public Button saveProfile;


    @Override
    public void initialize(URL location, ResourceBundle resources) {
        initProfileTab();
        initRepondersTab();
        initActivityTab();
    }

    private void initActivityTab() {
        //insert here. if log file does not exist, do something.
//        BufferedReader br = null;
//        try {
//            br = new BufferedReader(new FileReader(selectCardController.driveLetter + "log.txt"));
//            String temp = br.readLine();
//        } catch (IOException e) {
//        } finally {
//            try {
//                br.close();
//            } catch (IOException e) {
//            }
//        }
    }

    private void initRepondersTab() {
        //insert here. if respondents file does not exist, do something.
        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(selectCardController.driveLetter + "respondents.txt"));
            String temp = br.readLine();
        } catch (IOException e) {
        } finally {
            try {
                br.close();
            } catch (IOException e) {
            }
        }
    }

    private void initProfileTab() {
        //insert here. if login file does not exist, do something.
        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(selectCardController.driveLetter + "login.txt"));
            String temp = br.readLine();
            usernameHello.setText(temp + "!");
            usernameLogin.setText(temp);
            temp = br.readLine();
            passwordLogin.setText(temp);
            confirmLogin.setText(temp);
        } catch (IOException e) {
        } finally {
            try {
                br.close();
            } catch (IOException e) {
            }
        }

        genderProfile.getItems().add("male");
        genderProfile.getItems().add("female");
        try{
            br = new BufferedReader(new FileReader(selectCardController.driveLetter + "profile.txt"));
            firstNameProfile.setText(br.readLine());
            LastNameProfile.setText(br.readLine());
            genderProfile.setValue(br.readLine());
            ageProfile.setText(br.readLine());
            addressProfile.setText(br.readLine());
            contactNumberProfile.setText(br.readLine());
        } catch (IOException e){
        } finally {
            try {
                br.close();
            } catch (IOException e){
            }
        }
    }

    public void onEditLogin(ActionEvent actionEvent) {
        usernameLogin.setEditable(true);
        passwordLogin.setEditable(true);
        confirmLogin.setEditable(true);
        saveLogin.setDisable(false);
        feedbackLabelLogin.setText("");
    }

    public void onSaveLogin(ActionEvent actionEvent) {
        if(usernameLogin.getText().isEmpty() || passwordLogin.getText().isEmpty() || confirmLogin.getText().isEmpty()){
            feedbackLabel.setText("All fields are required!");
        } else if (!passwordLogin.getText().equals(confirmLogin.getText())){
            feedbackLabelLogin.setText("Passwords do not match!");
        } else {
            BufferedWriter writer = null;
            try {
                writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(selectCardController.driveLetter + "login.txt"), "utf-8"));
                writer.write(usernameLogin.getText());
                writer.newLine();
                writer.write(passwordLogin.getText());
                writer.close();

                feedbackLabelLogin.setText("Login Credentials was successfully modified!");
                usernameLogin.setEditable(false);
                passwordLogin.setEditable(false);
                confirmLogin.setEditable(false);
                saveLogin.setDisable(true);
            } catch (Exception e) {
            }
        }

    }

    public void onEditProfile(ActionEvent actionEvent) {
        firstNameProfile.setEditable(true);
        LastNameProfile.setEditable(true);
        genderProfile.setDisable(false);
        ageProfile.setEditable(true);
        addressProfile.setEditable(true);
        contactNumberProfile.setEditable(true);
        saveProfile.setDisable(false);
        feedbackLabel.setText("");
    }

    public void onSaveProfile(ActionEvent actionEvent) {
        if(firstNameProfile.getText().isEmpty() || LastNameProfile.getText().isEmpty() || ageProfile.getText().isEmpty() || addressProfile.getText().isEmpty() || contactNumberProfile.getText().isEmpty()){
            feedbackLabel.setText("All fields are required!");
        } else {
            try {
                Integer.parseInt(ageProfile.getText());

                BufferedWriter writer = null;
                try {
                    writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(selectCardController.driveLetter + "profile.txt"), "utf-8"));
                    writer.write(firstNameProfile.getText());
                    writer.newLine();
                    writer.write(LastNameProfile.getText());
                    writer.newLine();
                    writer.write(genderProfile.getValue().toString());
                    writer.newLine();
                    writer.write(ageProfile.getText());
                    writer.newLine();
                    writer.write(addressProfile.getText());
                    writer.newLine();
                    writer.write(contactNumberProfile.getText());
                    writer.close();

                    feedbackLabel.setText("Profile was successfully modified!");
                    firstNameProfile.setEditable(false);
                    LastNameProfile.setEditable(false);
                    genderProfile.setDisable(true);
                    ageProfile.setEditable(false);
                    addressProfile.setEditable(false);
                    contactNumberProfile.setEditable(false);
                    saveProfile.setDisable(true);
                } catch (Exception e) {
                }
            } catch (Exception e) {
                feedbackLabel.setText("Your age is invalid!");
            }
        }
    }

    public void onSaveResponder(ActionEvent actionEvent) {
    }

    public void onDeleteResponder(ActionEvent actionEvent) {
    }

    public void onEditResponder(ActionEvent actionEvent) {
    }

    public void onAddResponder(ActionEvent actionEvent) {
    }
}
