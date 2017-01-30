import javax.swing.*;
import java.awt.*;
import static java.nio.file.StandardOpenOption.*;
import java.nio.file.*;
import java.io.*;
import java.util.Scanner;
import java.util.*;

     import java.awt.*;
     import java.applet.Applet;

public class gui extends JFrame {
     
	public static void main(String[] args){

        //Schedule a job for the event dispatch thread:
        //creating and showing this application's GUI.
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
	}	
	public gui(String name) {
        super(name);
        setResizable(false);
    }

    public void addComponentsToPane(final Container pane) {
    	JPanel panel = new JPanel(new GridLayout(120,120));
    	JScrollPane scrollPane = new JScrollPane();
    	panel.setPreferredSize(new Dimension(800, 800));
    	Scanner scanner ;
    	try{
    		scanner = new Scanner(new File("mapa.txt"));
			int [][] Dados = new int [120][120];
			int i = 0;
			int aux = -1;
			String entradaString;
			while(scanner.hasNextInt()){
				entradaString = new String();
			   	for (int j =0 ; j < 120 ; j++) {
			   		aux =scanner.nextInt();
				   	entradaString +=  ""+aux;
			   	}
			   	panel.add(new Label(entradaString));
			   	i++;
			}
	    }catch(Exception e){

		}
		
        panel.setFont(new Font("TimesRoman", Font.PLAIN, 9));
		//panel.add(scrollPane, BorderLayout.CENTER);
        pane.add(panel,  BorderLayout.CENTER);
    }

	public static void createAndShowGUI(){
		gui frame = new gui("Mapa");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.addComponentsToPane(frame.getContentPane());
        frame.pack();
        frame.setVisible(true);
	}
}