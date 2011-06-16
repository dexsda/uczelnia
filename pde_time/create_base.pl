#! /usr/bin/perl -w
use strict;			 # Use strict typechecking
my $light=1;
package main;
use OpenGL qw(:all);	# Use the OpenGL module
use OpenGL::Image;
use OpenGL::Shader;
use List::Util qw[min max];
eval 'use Time::HiRes qw( gettimeofday )';
my $hasHires = !$@;
sub ftime{return $hasHires ? gettimeofday() : time()}

# ASCII constant for the escape key
use constant ESCAPE => 27;

my $appBench={};
my $campos={anglex=>0,angley=>0,eyedist=>-2.46};
my $bool=0;
my $wireframe=0;
my $window;
my $_width=$ARGV[0]*6+6;
my $_height=$ARGV[0]*6+6;
my $debug=1;
my $angle_x=0;
my $angle_y=0;
my $frames=0;
my $boundary=100;
my $_mincol=[0,0,1];
my $_maxcol=[1,0,0];
my $rate=10;
my $animate=0;

sub set_viewport_scale{
	my ($scale)=@_;
	glViewport(0, 0, $_width/$scale, $_height/$scale); #reset viewport and perspective transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity;
	gluPerspective(45.0, $_width/$_height, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity;
}

# A general GL initialization function 
sub InitGL {
	my ($width, $height) = @_;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0); #clear depth buffer
	glDepthFunc(GL_LESS); #depth test
	glEnable(GL_DEPTH_TEST); #enable depth testing
	glShadeModel(GL_SMOOTH); #smooth color shading
	glMatrixMode(GL_PROJECTION); #select projection matrix
	glLineWidth(2);
	glPointSize(2);
	glLoadIdentity; #restore matrix to original state
	# Calculate the aspect ratio of the Window
	gluPerspective(45.0, $width/$height, 0.1, 100.0);
	# Reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity;
}

sub ReSizeGLScene {
	my ($width, $height) = @_;
	# Prevent divide by zero error if window is too small
	if ($height == 0) { $height = 1; }
	# Reset the current viewport and perspective transformation
	glViewport(0, 0, $width, $height); #reset viewport and perspective transformation
	# Re-initialize the window (same lines from InitGL)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity;
	gluPerspective(45.0, $width/$height, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity;
	($_width,$_height)=($width,$height);
	printf "%dx%d\n",$_width,$_height if $debug;
}

sub drawFullScreenQuad{
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity(); 
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix();
	glLoadIdentity();
	glBegin (GL_QUADS); 
	glTexCoord2f(0, 0); glVertex3i(-1, -1, -1); 
	glTexCoord2f(1, 0); glVertex3i(1, -1, -1); 
	glTexCoord2f(1, 1); glVertex3i(1, 1, -1); 
	glTexCoord2f(0, 1); glVertex3i(-1, 1, -1); 
	glEnd();
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix();
}

sub saveImage {
	my($file) = @_;
	my $frame = new OpenGL::Image(width=>$_width, height=>$_height);
	my($fmt,$size) = $frame->Get('gl_format','gl_type');

	glReadPixels_c( 0, 0, $_width, $_height, $fmt, $size, $frame->Ptr() );
	$frame->Save($file);
}

# The main drawing function.
my @data=();
my $len=$ARGV[0];
my $n=0;
sub DrawGLScene {
	$frames++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if($wireframe==1){
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	} elsif ($wireframe==2) {
		glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}
	glLoadIdentity;
	glTranslatef(0.0,0.0,$campos->{eyedist});
	glRotatef($campos->{angley},1,0,0);
	glRotatef($campos->{anglex},0,0,1);
	my $step=2/($len-1);
	for(my $i=1; $i<$len; $i++){
		for(my $j=1; $j<$len; $j++){
			glBegin(GL_QUADS);{
				glColor3f(gradient(0,$boundary,abs($data[$i-1][$j-1]),$_mincol,$_maxcol));
				glVertex3f($step*($j-1)-1,1-$step*($i-1),$bool?$data[$i-1][$j-1]/100:0);
				glColor3f(gradient(0,$boundary,abs($data[$i-1][$j]),$_mincol,$_maxcol));
				glVertex3f($step*($j)-1,1-$step*($i-1),$bool?$data[$i-1][$j]/100:0);
				glColor3f(gradient(0,$boundary,abs($data[$i][$j]),$_mincol,$_maxcol));
				glVertex3f($step*($j)-1,1-$step*($i),$bool?$data[$i][$j]/100:0);
				glColor3f(gradient(0,$boundary,abs($data[$i][$j-1]),$_mincol,$_maxcol));
				glVertex3f($step*($j-1)-1,1-$step*($i),$bool?$data[$i][$j-1]/100:0);
			} glEnd;
		}
	}
	glutSwapBuffers;
	if($animate && $frames % $rate==0){
		$n++;
		$n=scalar(@data)-1 if $n>=scalar(@data);
	}
}

sub save_to_file {
	open FILE,">infile.txt";
	for my $i (0 .. $ARGV[0]-1){
		for my $j (0 .. $ARGV[0]-1){
			printf FILE "%8.4f ",$data[$i][$j];
		}
		print FILE "\n";
	}
}

my $factor=1;
my $change=1;
sub keyPressed {
	my ($key, $x, $y) = @_;
	if ($key == ESCAPE) {
		# Shut down our window
		$appBench->{end}=ftime();
		printf "FPS: %f\n",$frames/($appBench->{end}-$appBench->{start});
		glutDestroyWindow($window);
		exit(0);
	} elsif ($key == 60) {
		$rate+=1;
	} elsif ($key == 62) {
		$rate-=1;
	} elsif ($key == 112) {
		$bool=!$bool;
	} elsif ($key == 119) {
		$wireframe=1;
	} elsif ($key == 109) {
		$wireframe=2;
	} elsif ($key == 87 || $key == 77){
		$wireframe=0;
	} elsif ($key == ord("+")){
		$factor++;
	} elsif ($key == ord("i")){
		$change++;
	} elsif ($key == ord("d")){
		$change--;
	} elsif ($key == ord("-")){
		$factor--;
		$factor=0 if $factor<0;
	} elsif ($key == 32){
		for(my $a=-1*$factor;$a<1*$factor; $a++){
			for(my $b=-1*$factor;$b<1*$factor; $b++){
				if($y/6+$a>0 && $y/6+$a<$ARGV[0] && $x/6+$b>0 && $x/6+$b<$ARGV[0]){
					$data[$y/6+$b][$x/6+$a]+=10*$change;
					$data[$y/6+$b][$x/6+$a]=100 if $data[$y/6+$b][$x/6+$a]>100;
					$data[$y/6+$b][$x/6+$a]=-50 if $data[$y/6+$b][$x/6+$a]<-50;
				}
			}
		}
	} else {
		save_to_file();
	}
	$rate=1 if $rate==0;
	printf "key %d at %dx%d\n",$key,$x,$y if $debug;
}

sub gradient {
	my ($min,$max,$val,$lowcolor,$highcolor) = @_;
	my $factor = 0;
	if ($val >= $max){
		$factor = 1;
	} elsif ($val>$min) {
		$factor=($val-$min)/($max-$min);
	}
	my $red = $lowcolor->[0] + ($factor * ($highcolor->[0] - $lowcolor->[0]));
	my $green = $lowcolor->[1] + ($factor * ($highcolor->[1] - $lowcolor->[1]));
	my $blue = $lowcolor->[2] + ($factor * ($highcolor->[2] - $lowcolor->[2]));
	return ($red, $green, $blue)
}

sub mouseClicked {
	my ($button, $state, $x, $y) = @_;
	if($button==0 || $button==1){
		print "here";
		for(my $a=-1;$a<1; $a++){
			for(my $b=-1;$b<1; $b++){
				if($y/6+$a>0 && $y/6+$a<$ARGV[0] && $x/6+$b>0 && $x/6+$b<$ARGV[0]){
					$data[$y/6+$b][$x/6-$a]+=($button?-10:10);
				}
			}
		}
	} 
	printf "button %d (%d) at %dx%d\n",$button,$state,$x,$y if $debug;
	#refreshCam();
}

my %previous_frame=();
sub mousePressed {
	my ($x, $y) = @_;
	printf "mouse pointer moving over %dx%d\n",$x,$y if $debug;
}

sub init_data {
	for(1..$ARGV[0]){
		my @tmp=(0.0) x $ARGV[0];
		push @data, [@tmp];
	}
}

# --- Main program ---
glutInit();
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); #try GLUT_ALPHA too, RGB originally
glutInitWindowSize($_width, $_height);  
glutInitWindowPosition(0, 0);  
$window = glutCreateWindow("PDEDisplay 0.1");  
glutDisplayFunc(\&DrawGLScene);  
#glutFullScreen;
glutIdleFunc(\&DrawGLScene);
glutReshapeFunc(\&ReSizeGLScene);
glutKeyboardFunc(\&keyPressed);
glutMouseFunc(\&mouseClicked);
glutMotionFunc(\&mousePressed);

# Initialize our window.
InitGL($_width, $_height);
# Start Event Processing Engine
$appBench->{start}=ftime();
init_data();
glutMainLoop;

return 1;
