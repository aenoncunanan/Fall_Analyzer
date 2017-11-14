package dlsu.Utils;

import javax.swing.filechooser.FileSystemView;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by aenon on 09/11/2017.
 */
public class getExternalDevices {

    public final List<String> storages = new ArrayList<String>();
    public final List<String> storageLetters = new ArrayList<String>();

    public void getDevices(){

        FileSystemView fileSystemView = FileSystemView.getFileSystemView();

        File[] f = File.listRoots();
        for (int i = 0; i < f.length; i++)
        {
            if (!fileSystemView.getSystemDisplayName(f[i]).equals("")){
                storages.add(fileSystemView.getSystemDisplayName(f[i])); //Disk name and Disk Letter
                storageLetters.add(String.valueOf(f[i])); //Disk Letter
            }
        }
    }
}
