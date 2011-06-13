#! /usr/bin/perl -w
use strict;			 # Use strict typechecking
my $campos={};
my $light=1;
$campos->{angle}=0;
$campos->{dist}=10;
$campos->{eyex}=0;
$campos->{eyey}=0;
$campos->{eyez}=10;
package planet;
use OpenGL qw(:all);	# Use the OpenGL module
use OpenGL::Image;
use OpenGL::Shader;
my $GSL_CONST_MKSA_GRAVITATIONAL_CONSTANT=6.673e-11;
sub new ($$$$$$$$) {
	my ($class, $_pos, $_v, $mass, $radius, $texture, $rot, $main) = @_;
	my $self={};
	$self->{pos}=$_pos;
	$self->{v}=$_v;
	$self->{m}=$mass;
	$self->{r}=$radius;
	$self->{rot}=$rot;
	$self->{main}=$main;
	$self->{currot}=0;
	my $tex = new OpenGL::Image(engine=>'Magick',source=>$texture);
	my($ifmt,$fmt,$type) = $tex->Get('gl_internalformat','gl_format','gl_type');
	my($_w,$_h) = $tex->Get('width','height'); 
	($self->{tex}) = glGenTextures_p(1);
	#mipmap
	glBindTexture(GL_TEXTURE_2D, $self->{tex});
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps_c(GL_TEXTURE_2D, $ifmt, $_w, $_h, $fmt, $type, $tex->Ptr());
	glBindTexture(GL_TEXTURE_2D,0);
	bless ($self, $class);
	return $self;
}

sub step_rot {
	my ($self)=@_;
	$self->{currot}+=$self->{rot}
}

sub step_euler {
	my ($self,$step)=@_;
	$self->{pos}[0]=$self->{pos}[0]+$step*$self->{v}[0];
	$self->{pos}[1]=$self->{pos}[1]+$step*$self->{v}[1];
	$self->{pos}[2]=$self->{pos}[2]+$step*$self->{v}[2];
	my $r=sqrt($self->{pos}[0]**2+$self->{pos}[1]**2+$self->{pos}[2]**2);
	$self->{v}[0]=$self->{v}[0]-($step*$GSL_CONST_MKSA_GRAVITATIONAL_CONSTANT*$self->{main}->{m}*$self->{pos}[0]/$r**3);
	$self->{v}[1]=$self->{v}[1]-($step*$GSL_CONST_MKSA_GRAVITATIONAL_CONSTANT*$self->{main}->{m}*$self->{pos}[1]/$r**3);
	$self->{v}[2]=$self->{v}[2]-($step*$GSL_CONST_MKSA_GRAVITATIONAL_CONSTANT*$self->{main}->{m}*$self->{pos}[2]/$r**3);
}

sub init_rk2 {
	my ($self,$step)=@_;
	open PIPE,sprintf "./grav %f %f %f %f %f %f %f %f |",@{$self->{pos}},@{$self->{v}},$step,$self->{main}->{m};
}

sub step_rk2 {
	my ($self)=@_;
	my $useful = <PIPE>;
	if($useful=~m/^(\S+)\s(\S+)\s(\S+)$/g){
		$self->{pos}[0]=$1;
		$self->{pos}[1]=$2;
		$self->{pos}[2]=$3;
	}
}

sub printout {
	my $self=shift;
	printf "(%f,%f,%f)[%f,%f,%f] R: %f, M: %f, ROT: %f\n",@{$self->{pos}},@{$self->{v}},$self->{r},$self->{m},$self->{rot};
}
sub draw {
	my $self=shift;
	glLoadIdentity;
	gluLookAt($campos->{eyex},$campos->{eyey},$campos->{eyez},0,0,0, 0.0, 1.0, 0.0);
	if($self->{main}!=1 && $light){
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT1);
		glLightfv_s(GL_LIGHT1, GL_POSITION, pack "f4",@{$self->{main}->{pos}},1.0);
	} else {
		glDisable(GL_LIGHTING);
	}
	glTranslatef(@{$self->{pos}});
	glRotatef(-90,1.0,0.0,0.0);
	glRotatef($self->{currot},0.0,0.0,1.0);
	glBindTexture(GL_TEXTURE_2D, $self->{tex});
	my $quad=gluNewQuadric();
	gluQuadricDrawStyle($quad, GLU_FILL);
	gluQuadricTexture($quad, GL_TRUE);
	gluSphere( $quad , $self->{r}, 40 , 40 );
}

package main;
use OpenGL qw(:all);	# Use the OpenGL module
use OpenGL::Image;
use OpenGL::Shader;
#use Math::GSL::ODEIV;
eval 'use Time::HiRes qw( gettimeofday )';
my $hasHires = !$@; # Only appBench will be valid if no HiRes
sub ftime{return $hasHires ? gettimeofday() : time()}

# ASCII constant for the escape key
use constant ESCAPE => 27;

my $appBench={};
my $window;
my $_width=768;
my $_height=768;
my $debug=1;
my $angle_x=0;
my $angle_y=0;
my $qual=0;
my $frames=0;
my $shader;
my $shader2;
my @fbotextures;
my @framebuffers;
my @renderbuffers;
my $sun;
my $sun_bloom;
my $earth;
my $earth2;
my $skybox_tex;
my $rate=3;
#my $thing=Math::GSL::ODEIV::gsl_odeiv_system->new();
#$thing->swig_function_set(0);

sub init_single_fbo {
	my ($x,$w,$h)=@_;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[$x]);
	glBindTexture(GL_TEXTURE_2D, $fbotextures[$x]);
	glTexImage2D_c(GL_TEXTURE_2D, 0, GL_RGBA, $w, $h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, $fbotextures[$x], 0);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, $renderbuffers[$x]);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24,$w, $h);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, $renderbuffers[$x]);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

sub initFBO{
	@fbotextures = glGenTextures_p(10);
	@framebuffers = glGenFramebuffersEXT_p(10);
	@renderbuffers = glGenRenderbuffersEXT_p(10);

	init_single_fbo(0,$_width,$_height);
	init_single_fbo(1,$_width,$_height);
	init_single_fbo(2,$_width/2,$_height/2);
	init_single_fbo(3,$_width/2,$_height/2);
	init_single_fbo(4,$_width/4,$_height/4);
	init_single_fbo(5,$_width/4,$_height/4);
	init_single_fbo(6,$_width/8,$_height/8);
	init_single_fbo(7,$_width/8,$_height/8);
	init_single_fbo(8,$_width/16,$_height/16);
	init_single_fbo(9,$_width/16,$_height/16);
}

sub set_viewport_scale{
	my ($scale)=@_;
	glViewport(0, 0, $_width/$scale, $_height/$scale); #reset viewport and perspective transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity;
	gluPerspective(45.0, $_width/$_height, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity;
}

sub blur_fbo {
	my $x=shift;
	for(1 .. 3){
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[$x+1]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, $fbotextures[$x]);
		$shader->Enable();
		drawFullScreenQuad();
		$shader->Disable();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[$x]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, $fbotextures[$x+1]);
		$shader2->Enable();
		drawFullScreenQuad();
		$shader2->Disable();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}

# FBO texture renderer
sub renderFBO{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	$sun_bloom->draw();
	$earth->draw();
	$earth2->draw();
	#scales
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[2]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, $fbotextures[0]);
	set_viewport_scale(2);
	drawFullScreenQuad();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[4]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, $fbotextures[2]);
	set_viewport_scale(4);
	drawFullScreenQuad();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[6]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, $fbotextures[4]);
	set_viewport_scale(8);
	drawFullScreenQuad();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[8]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, $fbotextures[6]);
	set_viewport_scale(16);
	drawFullScreenQuad();
	blur_fbo(8);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[6]);
	#glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	set_viewport_scale(8);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc (GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, $fbotextures[8]);
	drawFullScreenQuad();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	blur_fbo(6);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[4]);
	set_viewport_scale(4);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc (GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, $fbotextures[6]);
	drawFullScreenQuad();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	blur_fbo(4);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[2]);
	set_viewport_scale(2);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc (GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, $fbotextures[4]);
	drawFullScreenQuad();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	blur_fbo(2);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, $framebuffers[0]);
	set_viewport_scale(1);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc (GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, $fbotextures[2]);
	drawFullScreenQuad();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	blur_fbo(0);
}

# Terminate FBO objects
sub termFBO {
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDeleteRenderbuffersEXT_p(@renderbuffers);
	glDeleteFramebuffersEXT_p(@framebuffers);
	glDeleteTextures_p(@fbotextures);
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
	glLoadIdentity; #restore matrix to original state
	# Calculate the aspect ratio of the Window
	gluPerspective(45.0, $width/$height, 0.1, 100.0);
	# Reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLightfv_s(GL_LIGHT1, GL_AMBIENT, pack "f4",0.1,0.1,0.1,1.0);
	glLightfv_s(GL_LIGHT1, GL_DIFFUSE, pack "f4",1.0,1.0,1.0,1.0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity;
}

sub loadSkybox {
	glEnable(GL_TEXTURE_2D);
	my $tex = new OpenGL::Image(engine=>'Magick',source=>'skybox/stars.png');
	my($ifmt,$fmt,$type) = $tex->Get('gl_internalformat','gl_format','gl_type');
	my($w,$h) = $tex->Get('width','height'); 
	($skybox_tex) = glGenTextures_p(1);
	#mipmap
	glBindTexture(GL_TEXTURE_2D, $skybox_tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	gluBuild2DMipmaps_c(GL_TEXTURE_2D, $ifmt, $w, $h, $fmt, $type, $tex->Ptr());
}

sub renderSkybox {
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ENABLE_BIT);
	gluLookAt(0,0,0,$campos->{eyex},-$campos->{eyey}, $campos->{eyez}, 0.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, $skybox_tex);
	glBegin(GL_QUADS); {
		glTexCoord2f(0, 0); glVertex3f(  0.5, -0.5, -0.5 );
		glTexCoord2f(1, 0); glVertex3f( -0.5, -0.5, -0.5 );
		glTexCoord2f(1, 1); glVertex3f( -0.5,  0.5, -0.5 );
		glTexCoord2f(0, 1); glVertex3f(  0.5,  0.5, -0.5 );
	} glEnd();

	glBindTexture(GL_TEXTURE_2D, $skybox_tex);
	glBegin(GL_QUADS); {
		glTexCoord2f(0, 0); glVertex3f(  0.5, -0.5,  0.5 );
		glTexCoord2f(1, 0); glVertex3f(  0.5, -0.5, -0.5 );
		glTexCoord2f(1, 1); glVertex3f(  0.5,  0.5, -0.5 );
		glTexCoord2f(0, 1); glVertex3f(  0.5,  0.5,  0.5 );
	} glEnd();

	glBindTexture(GL_TEXTURE_2D, $skybox_tex);
	glBegin(GL_QUADS); {
		glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5,  0.5 );
		glTexCoord2f(1, 0); glVertex3f(  0.5, -0.5,  0.5 );
		glTexCoord2f(1, 1); glVertex3f(  0.5,  0.5,  0.5 );
		glTexCoord2f(0, 1); glVertex3f( -0.5,  0.5,  0.5 );
	} glEnd();

	glBindTexture(GL_TEXTURE_2D, $skybox_tex);
	glBegin(GL_QUADS); {
		glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, -0.5 );
		glTexCoord2f(1, 0); glVertex3f( -0.5, -0.5,  0.5 );
		glTexCoord2f(1, 1); glVertex3f( -0.5,  0.5,  0.5 );
		glTexCoord2f(0, 1); glVertex3f( -0.5,  0.5, -0.5 );
	}glEnd(); 

	glBindTexture(GL_TEXTURE_2D, $skybox_tex);
	glBegin(GL_QUADS); {
		glTexCoord2f(0, 1); glVertex3f( -0.5,  0.5, -0.5 );
		glTexCoord2f(0, 0); glVertex3f( -0.5,  0.5,  0.5 );
		glTexCoord2f(1, 0); glVertex3f(  0.5,  0.5,  0.5 );
		glTexCoord2f(1, 1); glVertex3f(  0.5,  0.5, -0.5 );
	} glEnd();

	glBindTexture(GL_TEXTURE_2D, $skybox_tex);
	glBegin(GL_QUADS); {
		glTexCoord2f(0, 0); glVertex3f( -0.5, -0.5, -0.5 );
		glTexCoord2f(0, 1); glVertex3f( -0.5, -0.5,  0.5 );
		glTexCoord2f(1, 1); glVertex3f(  0.5, -0.5,  0.5 );
		glTexCoord2f(1, 0); glVertex3f(  0.5, -0.5, -0.5 );
	} glEnd();

	glPopAttrib();
	glPopMatrix();
	glDepthMask(GL_TRUE);
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
my $counter="0000";
sub DrawGLScene {
	$frames++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderFBO();
	glLoadIdentity;
	renderSkybox();
	$sun->draw();
	$earth->draw();
	$earth2->draw();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, $fbotextures[1]);
	drawFullScreenQuad();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	
	glutSwapBuffers;
	if($frames % $rate==0){
		$earth->step_rot();
		$earth2->step_rot();
		$sun->step_rot();
		$sun_bloom->step_rot();
		$earth->step_euler(0.01);
		#$earth2->step_rk2();
		$earth2->step_euler(0.01);
		#$earth->printout();
	}
	#if($frames % ($rate*5) == 0){
	#	saveImage("capture/comp".$counter);
	#	$counter++;
	#}
}

sub refreshCam {
	$campos->{dist}=1.2 if $campos->{dist}<1.2;
	$campos->{angle}%=360;
	$campos->{eyex}=sin(0.0174532925*$campos->{angle})*$campos->{dist};
	$campos->{eyez}=cos(0.0174532925*$campos->{angle})*$campos->{dist};
}

sub keyPressed {
	my ($key, $x, $y) = @_;
	if ($key == ESCAPE) {
		# Shut down our window
		$appBench->{end}=ftime();
		printf "FPS: %f\n",$frames/($appBench->{end}-$appBench->{start});
		glutDestroyWindow($window);
		exit(0);
	} elsif ($key == 48) {
		$qual=0;
	} elsif ($key == 49) {
		$qual=1;
	} elsif ($key == 50) {
		$qual=2;
	} elsif ($key == 105) {
		$light=0;
	} elsif ($key == 73) {
		$light=1;
	} elsif ($key == 104) {
		$campos->{angle}-=1;
	} elsif ($key == 106) {
		$campos->{eyey}-=1;
	} elsif ($key == 107) {
		$campos->{eyey}+=1;
	} elsif ($key == 108) {
		$campos->{angle}+=1;
	} elsif ($key == 43) {
		$campos->{dist}-=1;
	} elsif ($key == 45) {
		$campos->{dist}+=1;
	} elsif ($key == 60) {
		$rate+=1;
	} elsif ($key == 62) {
		$rate-=1;
	}
	$rate=1 if $rate==0;
	refreshCam();
	printf "key %d at %dx%d\n",$key,$x,$y if $debug;
}

sub mouseClicked {
	my ($button, $state, $x, $y) = @_;
	printf "button %d (%d) at %dx%d\n",$button,$state,$x,$y if $debug;
	if($button == 3 && $state == 0){
		$campos->{eyey}+=1;
	}elsif($button == 4 && $state == 0){
		$campos->{eyey}-=1;
	}
	refreshCam();
}

my %previous_frame=();
sub mousePressed {
	my ($x, $y) = @_;
	if(not exists $previous_frame{x}) {
		$previous_frame{x}=$x;
		$previous_frame{y}=$y;
	} else {
		$campos->{angle}+=$x-$previous_frame{x};
		$previous_frame{x}=$x;
		$campos->{dist}+=$y-$previous_frame{y};
		$previous_frame{y}=$y;
	}
	refreshCam();
	printf "mouse pointer moving over %dx%d\n",$x,$y if $debug;
}

# --- Main program ---
glutInit(@ARGV);
glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); #try GLUT_ALPHA too, RGB originally
glutInitWindowSize($_width, $_height);  
glutInitWindowPosition(0, 0);  
$window = glutCreateWindow("GravSim 0.0");  
glutDisplayFunc(\&DrawGLScene);  
#glutFullScreen;
glutIdleFunc(\&DrawGLScene);
glutReshapeFunc(\&ReSizeGLScene);
glutKeyboardFunc(\&keyPressed);
glutMouseFunc(\&mouseClicked);
glutMotionFunc(\&mousePressed);

# Initialize our window.
InitGL($_width, $_height);
loadSkybox();
$sun=planet->new([0,0,0],[0,0,0],100000000000,1,"sun.png",1,1);
$sun_bloom=planet->new([0,0,0],[0,0,0],100000000000,1,"sun_bloom.png",1,1);
$earth=planet->new([2,0,0],[0,0,2],10,0.2,"earth.png",1,$sun);
#$earth2=planet->new([2,0,0],[0,0,2],10,0.2,"mars.png",1,$sun);
#$earth2->init_rk2(0.1);
$earth2=planet->new([2*1.665,0,0],[0,0,2/1.2368],10,0.2,"mars.png",1,$sun);
# Start Event Processing Engine
initFBO();
$shader = new OpenGL::Shader('GLSL');
print $shader->LoadFiles("frag.glsl","vert.glsl");
$shader2 = new OpenGL::Shader('GLSL');
print $shader2->LoadFiles("frag2.glsl","vert2.glsl");
$appBench->{start}=ftime();
glutMainLoop;  

return 1;
