require 'spec_helper'

describe Spyglass::GUI::Window, gui: true do
  let(:window)  { Spyglass::GUI::Window.new("Image preview") }
  let(:lena)    { Spyglass::Image.load(fixture_path('lena.jpg')) }

  describe '.new' do
    it 'should require one argument' do
      expect { Spyglass::GUI::Window.new }.to raise_error ArgumentError
      expect( window ).to be_a Spyglass::GUI::Window
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
