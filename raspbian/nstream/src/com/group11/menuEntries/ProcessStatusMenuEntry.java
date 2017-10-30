package com.group11.menuEntries;

import com.group11.ConsoleHandler;

import java.io.File;
import java.io.IOException;
import java.util.List;

public class ProcessStatusMenuEntry extends BaseMenuEntry {

    private Process[] mutuallyExclusiveWithProcesses;
    private Process process;

    private boolean isRunning;
    public boolean getIsRunning() { return isRunning; }

    public List<String> command;

    public ProcessStatusMenuEntry(String title, List<String> command) {
        super(title);
        this.command = command;
    }

    public void requiresStoppingOfProcesses(Process[] processes) {
        mutuallyExclusiveWithProcesses = processes;
    }

    @Override
    public String getTitle() {
        return String.format("%-55s %s", super.getTitle(), isRunning ? "[Running]" : "[Stopped]");
    }


    @Override
    public void action() {

        if (isRunning) {
            process.destroy();
        } else {
            if (mutuallyExclusiveWithProcesses != null) {
                for (Process process : mutuallyExclusiveWithProcesses) {
                    process.destroy();
                }
            }

            Runnable asyncTask = () -> {
                try {
                    process = new ProcessBuilder(command).redirectOutput(new File(String.format("%s.log", getTitle().replaceAll(" ", "_")))).start();
//                    ConsoleHandler.shared().printMessage(command.toString());
//                    process = new ProcessBuilder(command).inheritIO().start();
                    updateStatus(true);
                    process.waitFor();
                    updateStatus(false);
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            };
            new Thread(asyncTask).start();
        }
    }

   private void updateStatus(boolean status) {
        isRunning = status;
       ConsoleHandler.shared().reloadMenuAfter(0);
   }
}