package dlsu.Utils;

import dlsu.selectCardController;
import javafx.application.Platform;
import javafx.geometry.Insets;
import javafx.scene.Node;
import javafx.scene.control.*;
import javafx.scene.layout.GridPane;
import javafx.util.Pair;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Optional;

/**
 * Created by aenon on 14/11/2017.
 */
public class logInDialog {
    public Boolean logIn(){
        String loginusername = "";
        String loginpassword = "";
        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(selectCardController.driveLetter + "login.txt"));
            loginusername = br.readLine();
            loginpassword = br.readLine();
        } catch (IOException e) {
        } finally {
            try {
                br.close();
            } catch (IOException e) {
            }
        }

        // Create the custom dialog.
        Dialog<Pair<String, String>> dialog = new Dialog<>();
        dialog.setTitle("Login to continue!");
        dialog.setHeaderText("Please enter username and password to continue.");

//// Set the icon (must be included in the project).
//            dialog.setGraphic(new ImageView(this.getClass().getResource("Main.png").toString()));

// Set the button types.
        ButtonType loginButtonType = new ButtonType("Login", ButtonBar.ButtonData.OK_DONE);
        dialog.getDialogPane().getButtonTypes().addAll(loginButtonType, ButtonType.CANCEL);

// Create the username and password labels and fields.
        GridPane grid = new GridPane();
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(20, 150, 10, 10));

        TextField username = new TextField();
        username.setPromptText("Username");
        PasswordField password = new PasswordField();
        password.setPromptText("Password");

        grid.add(new Label("Username:"), 0, 0);
        grid.add(username, 1, 0);
        grid.add(new Label("Password:"), 0, 1);
        grid.add(password, 1, 1);

// Enable/Disable login button depending on whether a username was entered.
        Node loginButton = dialog.getDialogPane().lookupButton(loginButtonType);
        loginButton.setDisable(true);

// Do some validation (using the Java 8 lambda syntax).
        username.textProperty().addListener((observable, oldValue, newValue) -> {
            loginButton.setDisable(newValue.trim().isEmpty());
        });

        dialog.getDialogPane().setContent(grid);

// Request focus on the username field by default.
        Platform.runLater(() -> username.requestFocus());

// Convert the result to a username-password-pair when the login button is clicked.
        dialog.setResultConverter(dialogButton -> {
            if (dialogButton == loginButtonType) {
                return new Pair<>(username.getText(), password.getText());
            }
            return null;
        });

        Optional<Pair<String, String>> result = dialog.showAndWait();

        final String[] user = {""};
        final String[] pass = {""};
        result.ifPresent(usernamePassword -> {
            user[0] = usernamePassword.getKey();
            pass[0] = usernamePassword.getValue();
            //System.out.println("Username=" + usernamePassword.getKey() + ", Password=" + usernamePassword.getValue());
        });

        if (user[0].equals(loginusername) && pass[0].equals(loginpassword)){
            return true;
        } else{
            return false;
        }
    }
}
