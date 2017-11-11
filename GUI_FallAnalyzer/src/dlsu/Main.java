package dlsu;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        Parent root = FXMLLoader.load(getClass().getResource("SelectCard.fxml"));
        root.getStylesheets().addAll(this.getClass().getResource("style.css").toExternalForm());
        primaryStage.setTitle("Fall Analyzer | Select Card");
        primaryStage.setResizable(false);
        primaryStage.setScene(new Scene(root, 1024, 575.5));
        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
