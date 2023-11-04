package oguzhantopaloglu;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;



public class MyFrame extends JFrame {

    private TextField searchKey, dbTf;

    public MyFrame() {
        super();
        setResizable(false);
        setLayout(null);
        setTitle("Oguzhan Topaloglu, C19052025, Big Data Project");
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setSize(800, 500);

        JButton openHadoop = new JButton("Open Hadoop");
        openHadoop.setBounds(50, 50, 150, 20);
        openHadoop.addActionListener(this::openHadoop);
        getContentPane().add(openHadoop);

        searchKey = new TextField("");
        searchKey.setBounds(250, 50, 150, 20);
        JButton search = new JButton("Search Questions");
        search.addActionListener(this::search);
        search.setBounds(250, 100, 150, 20);
        getContentPane().add(searchKey);
        getContentPane().add(search);

        JButton max = new JButton("Max");
        max.addActionListener(this::max);
        max.setBounds(550, 50, 150, 20);
        JButton min = new JButton("Min");
        min.addActionListener(this::min);
        min.setBounds(550, 150, 150, 20);
        JButton avg = new JButton("Avg");
        avg.addActionListener(this::avg);
        avg.setBounds(550, 250, 150, 20);
        JButton std = new JButton("Std");
        std.addActionListener(this::std);
        std.setBounds(550, 350, 150, 20);
        getContentPane().add(max);
        getContentPane().add(min);
        getContentPane().add(avg);
        getContentPane().add(std);

        dbTf = new TextField("/input");
        dbTf.setBounds(350, 350, 150, 20);
        getContentPane().add(dbTf);

        setVisible(true);
    }


    private void openHadoop(ActionEvent actionEvent) {
        command("C:\\Hadoop\\hadoop-3.3.5\\sbin\\start-all.cmd");
    }
    private void search(ActionEvent actionEvent) {
        command(String.format("hadoop jar ./myjobs.jar oguzhantopaloglu.Contains %s /out1 %s", dbTf.getText(), searchKey.getText()));
    }
    private void max(ActionEvent actionEvent) {
        command(String.format("hadoop jar ./myjobs.jar oguzhantopaloglu.Max %s /out2", dbTf.getText()));
    }
    private void min(ActionEvent actionEvent) {
        command(String.format("hadoop jar ./myjobs.jar oguzhantopaloglu.Min %s /out3", dbTf.getText()));
    }
    private void std(ActionEvent actionEvent) {
        command(String.format("hadoop jar ./myjobs.jar oguzhantopaloglu.Std %s /out4", dbTf.getText()));
    }
    private void avg(ActionEvent actionEvent) {
        command(String.format("hadoop jar ./myjobs.jar oguzhantopaloglu.Avg %s /out5", dbTf.getText()));
    }


    private void command(String command) {
        try {
            ProcessBuilder builder = new ProcessBuilder("cmd.exe", "/c", command);
            builder.redirectErrorStream(true);
            Process p = builder.start();
            p.waitFor();
        }
        catch (Exception e) {}
    }




}
