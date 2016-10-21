using UnityEngine;
using System.Collections;
using System.Diagnostics; // to use Process class
using System.IO; // to use Path class

//Template to lauch Python file
//Set the adress of the file starting from the Assets fold of the current Unity project
//Consol feedbacks are printed into the Unity debug consol

//public class PythonLauncher : 
public class PythonLauncher {

	// Use this for initialization
	public PythonLauncher () {
		//callPythonScript ("Main.py", "../../../Python");
		callPythonScript ("OSC_communication.py", "../../../Python");
	}

	//[MenuItem("MyGame/Downscale Reference Textures")]
	private static void callPythonScript(string fileName, string fileAdd) {
		//Note: the address is define starting from the Assets fold of the current Unity project

		// using System.Diagnostics;
		Process p = new Process();
		p.StartInfo.FileName = "python";	
		p.StartInfo.Arguments = fileName;    
		// Pipe the output to itself - we will catch this later
		p.StartInfo.RedirectStandardError=true;
		p.StartInfo.RedirectStandardOutput=true;
		p.StartInfo.CreateNoWindow = true;

		// Where the script lives
		//p.StartInfo.WorkingDirectory = Application.dataPath; 
		p.StartInfo.WorkingDirectory = Path.GetFullPath(Path.Combine(Application.dataPath, @fileAdd)); 
		p.StartInfo.UseShellExecute = false;

		p.Start();
		// Read the output - this will show is a single entry in the console - you could get  fancy and make it log for each line - but thats not why we're here
		UnityEngine.Debug.Log( p.StandardOutput.ReadToEnd() );
		p.WaitForExit();
		p.Close();
	}
}
