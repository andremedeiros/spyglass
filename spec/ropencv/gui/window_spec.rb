require 'spec_helper'

describe OpenCV::GUI::Window do
  let(:window)  { OpenCV::GUI::Window.new("Image preview") }
  let(:lena)    { OpenCV::Image.load(fixture_path('lena.jpg')) }

  describe '.new' do
    it 'should require one argument' do
      expect { OpenCV::GUI::Window.new }.to raise_error ArgumentError
      expect( window ).to be_a OpenCV::GUI::Window
    end
  end

  describe 'accessors' do
    describe '#title' do
      it 'should return the correct title' do
        expect( window.title ).to eq("Image preview")
      end
    end
  end
end
