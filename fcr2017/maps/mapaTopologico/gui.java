import javax.swing.*;
import java.awt.*;
import static java.nio.file.StandardOpenOption.*;
import java.nio.file.*;
import java.io.*;
import java.util.Scanner;
import java.util.*;


import java.awt.*;

import java.awt.event.*;

import javax.swing.*;

import javax.swing.border.*;

import javax.swing.text.AttributeSet;
import javax.swing.text.SimpleAttributeSet;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyleContext;





     import java.awt.*;
     import java.applet.Applet;

public class gui extends JFrame {

    public static gui frame;
    public static JPanel panel;
    private JPanel topPanel;
    private JTextPane tPane;


    public gui(String name) {
        super(name);
        setResizable(false);
    }

    public static void main(String[] args){
        final String argumento = args[0];
        //Schedule a job for the event dispatch thread:
        //creating and showing this application's GUI.
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI(argumento);
                
            }
        });
    }   
 

    public static void createAndShowGUI(final String arg){
        if(panel == null)
            frame = new gui("Mapa");
        else{
            frame.remove(panel);
            System.out.println("Atualizei!");
        }
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setTitle(arg);
        frame.addComponentsToPane(arg);
        frame.pack();
        frame.setVisible(true);
    }


    private void appendToPane(JTextPane tp, String msg, Color c)
    {
        StyleContext sc = StyleContext.getDefaultStyleContext();
        AttributeSet aset = sc.addAttribute(SimpleAttributeSet.EMPTY, StyleConstants.Foreground, c);

        aset = sc.addAttribute(aset, StyleConstants.FontFamily, "Lucida Console");
        aset = sc.addAttribute(aset, StyleConstants.Alignment, StyleConstants.ALIGN_JUSTIFIED);

        int len = tp.getDocument().getLength();
        tp.setCaretPosition(len);
        tp.setCharacterAttributes(aset, false);
        tp.replaceSelection(msg);
    }

    public void addComponentsToPane(final String arg) {
        panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
        Scanner scanner ;

        try{
            scanner = new Scanner(new File(arg));
            int [][] Dados ;
            int auxiliarDados;
            int i = 0;
            int lines = 0;
            int columns = 0;
            int aux = -1;
            String entradaString;
            columns = scanner.nextLine().split("\t").length;
            scanner = new Scanner(new File(arg));
            panel.add(new Label(arg));
            topPanel = new JPanel();        
            setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            setLocationRelativeTo(null);            

            EmptyBorder eb = new EmptyBorder(new Insets(10, 10, 10, 10));

            tPane = new JTextPane();                
            tPane.setBorder(eb);
            //tPane.setBorder(BorderFactory.createLineBorder(Color.DARK_GRAY));
            tPane.setMargin(new Insets(5, 5, 5, 5));

            topPanel.add(tPane);
            while (scanner.hasNextLine()) {
                scanner.nextLine();
                lines++;
            }
            Dados = new int[lines][columns];
            scanner = new Scanner(new File(arg));
            while(scanner.hasNextInt()){
                entradaString = new String();
                for (int j =0 ; j < columns ; j++) {
                    aux =scanner.nextInt();
                    if (aux == 1) {
                        appendToPane(tPane, "*", Color.BLUE);
                    }else if(aux == 0){
                        appendToPane(tPane, "*", Color.BLACK);
                    }else if(aux == -1){
                        appendToPane(tPane, "*", Color.RED);
                    }else if(aux == -2){
                        appendToPane(tPane, "*", Color.MAGENTA);
                    }
                    entradaString +=  ""+aux;
                }
                appendToPane(tPane, "\n", Color.MAGENTA);
                i++;
            }
        //frame.add(topPanel);
        }catch(Exception e){
            System.out.println("\tERRO NAO ACHADO O MAPA = "+arg + "\n");
        }
        
        //panel.setFont(new Font("TimesRoman", Font.PLAIN, 12));
        //panel.add(scrollPane, BorderLayout.CENTER);
        this.getContentPane().add(topPanel,  BorderLayout.CENTER);
    }

}
